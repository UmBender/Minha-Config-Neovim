---
status: done
created: 2026-10-02
updated: 2026-10-03
tags: [task, ui, performance]
---
# T-022 Snappier editor

## Goal
After [[T-014 Performance]] the editor still feels clunky: startup, moving around in a file and
switching files all feel laggy. Analyse the config again and make it faster, **keeping the look**
(Kanagawa Dragon, BENDER VIM dashboard, bubbles lualine, rainbow indent/brackets, dropbar winbar),
per [[D-012 Performance budget]].

## Requirements
- Measure before changing anything: startup (to first screen and to VeryLazy done), cost per cursor
  move / scroll (`j`, `<C-d>`, `G`), cost of opening another file (picker, `:e`, buffer switch).
- Find what each of those spends time on (autocmds, plugins, Treesitter, LSP) and cut it.
- Any feature removed or changed that is visible to the user is listed here and is the user's call.
- Regression tests for each tweak (`tests/nvim/perf_test.lua`), full `tests/run.py` green.

## Plan
- [x] Baseline: startup timings, per-keystroke autocmd profile, file-switch timing
- [x] Rank hotspots, decisions taken (see below)
- [x] Tests for the tweaks
- [x] Implement, re-measure, log numbers
- [x] [[D-018 Paint first, Treesitter after]] (supersedes part of [[D-012 Performance budget]]), [[UI and Colorscheme]]

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-022-snappier-editor`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Findings (baseline, 2026-10-03)
Harness: pty driver that sends real keys to `nvim a.cpp` (618-line C++ file, clangd attached) and
measures ms from key to first terminal byte (`lat`) and until the screen is quiet for 25 ms (`done`),
plus startup marks (UIEnter, VeryLazy done). Ablation = same run with one plugin disabled (2 runs each).
Scripts were in the session scratchpad, not committed; the method is described here so it can be redone.

- **The machine is short on memory**: 7.8 GB total, ~1 GB available during the runs. Startup ranged
  600-1475 ms for the *same* config depending on load, and the ablation job was stopped by the
  low-memory reaper after 10 of 14 configs (not done: bufferline, gitsigns, hipatterns/todo, clangd).
  Part of the "clunky" feel is likely swapping/load, not the config.
- **Startup**: ~110 ms is Neovim core waiting on terminal queries (pty artifact, see [[T-014 Performance]]).
  "opening buffers" is ~670 ms, of which ~380 ms is **compiling C++ Treesitter queries** before the
  first screen: `highlights` 260 ms (the C++ grammar has a ~18 ms base cost per query, so it can't be
  trimmed pattern by pattern), `injections` 18-47, `folds` 35, `rainbow-delimiters` 34. After UIEnter:
  `context` 51 ms. Not compiled at startup (good): `textobjects` 145, `locals` 76, `indents` 32.
- **Per keystroke**: nvim CPU is tiny (0.2-2 ms per key) and first paint is fast (`j` ~4 ms,
  `<C-d>` ~13 ms, `G` ~18 ms). What keeps the screen moving afterwards is the **smear cursor**
  animation: with smear off, `done` for `G` 122 -> 34 ms, `w` 47 -> 7, `<C-d>` 88 -> 36,
  buffer switch 152 -> 56. Every other plugin (dropbar, treesitter-context, rainbow, tiny-inline-
  diagnostic, snacks indent/words, lualine) was within run-to-run noise. noice-off runs are invalid
  (cmdline behaves differently, the harness misreads it).
- **Switching files**: `:e` of an already-seen language ~10-35 ms, picker opens in ~25-30 ms. The
  slow part of a switch is again the smear animation (the cursor flies to the new position).

## Decisions (user's call, 2026-10-03)
1. Smear cursor: **removed** (options were: keep / much faster / no smear on big jumps / remove).
2. Startup Treesitter: **both** — first paint with Vim's regex syntax, and rainbow brackets + folds
   after the first screen. See [[D-018 Paint first, Treesitter after]].

## Changes
- `lua/config/lazy.lua`, `lua/plugins/ui.lua`: smear-cursor extra and spec removed.
- `lua/util/perf.lua`: `when_ready(buf, fn)` (first buffer of a language waits for the first screen,
  `VimEnter` + one tick; later `:e` one tick), `highlight`/`setup_highlight` (Treesitter start),
  `setup` also wraps LazyVim's `foldexpr` (flat until ready, then recomputes the window's folds).
- `lua/plugins/ui.lua`: snacks `quickfile.exclude` = every installed parser (+ latex), so the early
  paint uses regex syntax; nvim-treesitter `opts.highlight.enable = false` + `util.perf.setup_highlight`;
  rainbow-delimiters `condition` goes through `when_ready`.
- `tests/nvim/perf_test.lua`: the "smear is snappy" test is **deliberately replaced** by "no smear
  cursor" (the user chose removal); 7 new tests (when_ready ordering/cleanup, deferred highlight with
  regex syntax and no double start, quickfile exclude, treesitter spec, folds, rainbow). Mutations
  checked: `when_ready` never deferring (4 failures), folds not recomputed (1 failure).

## Results (pty, 618-line C++ file)
- First screen: **817 -> 241 ms** (UIEnter, median of 6 alternating runs vs a `git archive` of
  `main`). VeryLazy 928 -> 928. VeryLazy done 1085 -> 1156 ms (+70: regex syntax + one repaint;
  the query work itself just moved after the screen).
- Screen settled after a key (median of 3 runs, was -> now): `j` 21 -> 8 ms, `w` 47 -> 7,
  `<C-d>` 88 -> 37, `G` 122 -> 50, `gg` 128 -> 39, `:e b.cpp` 44 -> 30, `:b#` 152 -> 61.
  First paint after a key was already fast and is unchanged (`j` ~4 ms, `<C-d>` ~13 ms).
- End-to-end (real session): after startup the C++ buffer has Treesitter highlighting, rainbow and
  folds; a `.py` opened later shows regex syntax for one tick, then all three; a second `.cpp` gets
  them at once (Treesitter folds are async, as before).
- Not addressed (outside the config): memory pressure on the machine (see Findings).

## Log
- 2026-10-02: created.
- 2026-10-03: baseline + ablation (findings above). Stopped for the decisions above.
- 2026-10-03: decisions taken; implemented. A first real-session trace still compiled `highlights`
  before the first screen: the caller was snacks quickfile (fixed with `exclude`). FileType fires
  twice at startup (lazy.nvim replay), so the deferred start is guarded against a double start.
  Full `tests/run.py` green; re-measured (Results above).

## Related
- Decisions: [[D-018 Paint first, Treesitter after]], [[D-012 Performance budget]]
- Tasks: [[T-014 Performance]]
- Guides: [[UI and Colorscheme]]
