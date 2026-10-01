---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, ui, performance]
---
# T-014 Performance

## Goal
The smear cursor looks good but moves slowly: make it faster. Everything feels slow in general:
profile the config and optimize it, **keeping the look** (Kanagawa, dashboard, bubbles lualine,
rainbow indent/brackets, dropbar, sticky context, smear cursor).

## Plan
- [x] Measure: startup (`--startuptime`, LuaJIT profiler, per-query timing) and what runs on every
  cursor move / keystroke (autocmds on `CursorMoved`, `TextChanged`, `WinScrolled`, ...)
- [x] Smear cursor: snappier dynamics (upstream "faster smear" values) + ~140 fps (`ui.lua`)
- [x] Startup: stop LazyVim from compiling Treesitter queries only to check they exist
  (`lua/util/perf.lua`, installed from `options.lua`)
- [x] Runtime: smooth scroll off, indent scope animation off, trouble symbols in lualine off
- [x] Regression tests (`tests/nvim/perf_test.lua`)
- [x] Re-measured, [[UI and Colorscheme]] updated, [[D-012 Performance budget]]

## Findings (baseline, `a.cpp` = a 100-line template, pty session)
- Startup ~620 ms to `lazy.stats()`; about 120 ms of it is the pty not answering terminal queries
  (also there with `nvim --clean`), so not ours.
- ~75% of the Lua time is **Treesitter query compilation** for C++ (`vim.treesitter.query.parse`):
  `highlights` 253 ms, `textobjects` 144 ms, `context` 47 ms, `injections` 46 ms,
  `rainbow-delimiters` 37 ms, `folds` 35 ms, `indents` 31 ms. Highlights/injections/context/rainbow
  are needed for what's on screen. **`textobjects`, `folds` and `indents` are compiled by
  `LazyVim.treesitter.have_query`, which calls `query.get()` (full compile) just to know whether the
  query exists.** textobjects alone is the 161 ms `nvim-treesitter-textobjects` VeryLazy hit right
  after the first screen.
- Per cursor move: matchparen, dropbar, treesitter-context, snacks scope/words, tiny-inline-diagnostic,
  flash, smear, lualine, and **trouble's LSP document-symbols** (LazyVim adds a `symbols` lualine
  component when trouble is installed; it re-requests `documentSymbol` on every change). The winbar
  (dropbar) already shows the same breadcrumb, so it's duplicated work.
- **snacks.scroll** (smooth scrolling) is on: every `<C-d>`, `<C-u>`, `G`, search jump animates
  over ~250 ms, which reads as "slow" more than anything else.
- Smear defaults: stiffness 0.6 / trailing 0.45, damping 0.85, stops at 0.1 cell, 17 ms frames.

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-014-performance`
- [x] Tests written and failing for the right reason (5/5 failed: missing module, no smear spec, opts unset)
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-01: created, profiled (findings above).
- 2026-10-01: implemented. Result (median of 6 alternating runs, pty, `a.cpp`): `lazy.stats()`
  startup 548 -> 534 ms, time until VeryLazy is done **825 -> 655 ms**. textobjects/indents are no
  longer compiled at startup; `folds` still is (Treesitter foldexpr until clangd attaches, 35 ms).
  Per keystroke: trouble's `documentSymbol` requests and their `TextChanged` autocmds are gone.
- 2026-10-01: kept on purpose: noice (cmdline look), dropbar, treesitter-context, rainbow
  delimiters, snacks words (underlined references). Their cost is per-session query compilation
  or debounced, and they're part of the look.
- Not automatable: how fast the smear *feels*. Values are upstream's "faster smear"; if still slow,
  raise `stiffness`/`trailing_stiffness` towards 1 in `ui.lua`.

## Related
- Decisions: [[D-012 Performance budget]]
- Guides: [[UI and Colorscheme]], [[Plugins and Maintenance]]
