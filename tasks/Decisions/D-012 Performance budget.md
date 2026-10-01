---
status: accepted
date: 2026-10-01
tags: [decision, ui, performance]
---
# D-012 Performance budget

## Context
The editor felt slow and the smear cursor lagged behind ([[T-014 Performance]]). Profiling showed
that most startup time is Treesitter compiling C++ queries, part of it only to check that a query
exists, and that several features animated or recomputed things with no visible benefit.

## Decision
- **Keep the look, drop the waiting**: the visual identity stays (smear, rainbow indent/brackets,
  dropbar, sticky context, bubbles lualine), but anything that makes the user wait for an animation
  is off or fast: smooth scroll off, indent scope animation off, smear tuned to upstream's "faster"
  values at ~140 fps.
- **No duplicated work per keystroke**: the code context lives in the winbar (dropbar), so
  LazyVim's trouble `symbols` lualine component is off (`vim.g.trouble_lualine = false`).
- **Never compile a Treesitter query to test it exists**: `lua/util/perf.lua` replaces
  `LazyVim.treesitter.have_query` with a file lookup. Textobjects/indents are compiled only when
  used (~180 ms less until the editor is ready).
- Every tweak has a regression test in `tests/nvim/perf_test.lua`.

## Consequences
- Time until VeryLazy is done: ~825 ms -> ~655 ms (median of 6, pty, `a.cpp`).
- `lua/util/perf.lua` patches a LazyVim internal; if LazyVim renames `have_query`/`_queries`, the
  patch must follow (the test only covers the patch itself, not LazyVim's internals).
- Smooth scroll can still be toggled per session with `<leader>uS`.

## Related
- Tasks: [[T-014 Performance]]
