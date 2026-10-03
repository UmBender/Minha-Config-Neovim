---
status: accepted
date: 2026-10-03
tags: [decision, ui, performance]
---
# D-018 Paint first, Treesitter after

## Context
After [[T-014 Performance]] the editor still felt clunky ([[T-022 Snappier editor]]). Measuring real
keystrokes in a pty showed two things:
- Startup: the first screen waited ~380 ms on compiling C++ Treesitter queries (`highlights` 260 ms,
  plus injections, rainbow-delimiters, folds). `highlights` was compiled by **snacks quickfile**,
  which paints `nvim file` before plugins load but started Treesitter to do it. A C++ query has a
  ~18 ms base cost (huge grammar), so the query itself can't be trimmed.
- Moving around: first paint after a key is fast (`j` ~4 ms, `G` ~18 ms), but the **smear cursor**
  kept the screen animating for ~90 ms after every jump (`G` settled in 122 ms vs 34 ms without it).
  No other plugin had an effect above run-to-run noise.

## Decision
- **Smear cursor removed** (user's call over "faster smear" or "no smear on big jumps"). This
  supersedes the smear part of [[D-012 Performance budget]].
- **The first buffer of each language is painted before its Treesitter queries are compiled**
  (`lua/util/perf.lua`, `when_ready`): it shows Vim's regex syntax, flat folds and plain brackets,
  and right after the first screen (startup) or one tick later (a later `:e`) gets Treesitter
  highlighting, rainbow brackets and folds. Later buffers of the same language start at once,
  because queries are compiled once per session.
  - snacks quickfile excludes every installed parser (it paints with regex syntax);
  - LazyVim's highlighting is off (`opts.highlight.enable = false` on nvim-treesitter) and
    `util.perf.setup_highlight` starts it instead;
  - rainbow-delimiters `condition` and LazyVim's `foldexpr` (patched in `util.perf.setup`) wait for
    the language too.
- Every tweak keeps its regression test in `tests/nvim/perf_test.lua`.

## Consequences
- First screen (median of 6 alternating runs, pty, 618-line C++ file): **817 -> 241 ms**. Everything
  loaded (VeryLazy done): 1085 -> 1156 ms: the same query work runs after the screen, plus the regex
  syntax and one more repaint (~70 ms).
- After a jump the screen settles in `G` 122 -> 50 ms, `w` 47 -> 7, `<C-d>` 88 -> 37, buffer
  switch 152 -> 61 ms.
- For ~0.3 s after opening the first C++ file (and the first file of any other language) the colors
  are Vim's regex ones, then they switch to Treesitter's.
- `util.perf` now also patches LazyVim's `foldexpr` and relies on rainbow-delimiters'
  `lib.attach`; if those change upstream, the patch must follow.
- If smear is wanted back: re-add the `ui.smear-cursor` extra in `lua/config/lazy.lua` (the tuned
  values are in [[D-012 Performance budget]] / git history) and drop the test that forbids it.

## Related
- Tasks: [[T-022 Snappier editor]], [[T-014 Performance]]
- Decisions: [[D-012 Performance budget]] (partly superseded)
