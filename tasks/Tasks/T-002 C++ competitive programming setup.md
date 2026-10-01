---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp]
---
# T-002 C++ competitive programming setup

## Goal
Basic C++ setup focused on competitive programming.

## Plan
- [x] clangd (LazyVim `lang.clangd` extra) tuned for lone `.cpp` files
- [x] clang-format with a fallback `.clang-format` (4 spaces)
- [x] `lua/util/cp.lua`: compile to quickfix, compile & run in a float terminal, sanitizer build
- [x] CompetiTest (test cases + Competitive Companion)
- [x] `templates/cp.cpp` auto-inserted in new `*.cpp` files
- [x] Snippets: `fori`, `all`, `vread`, `yesno`
- [x] DAP with codelldb (`dap.core` extra)

## Log
- 2026-10-01: done in commit `5cf9593`. Verified in a real session: clangd attaches, `LOCAL` is
  defined, format on save, compile & run shows output.

## Related
- Decisions: [[D-002 clang-format with a fallback style file]], [[D-003 clangd flags for single-file C++]]
- Guides: [[Competitive Programming]], [[LSP, Formatting and Debugging]]
- Follow-up: [[T-005 Notebook-based templates]]
