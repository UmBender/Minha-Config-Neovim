---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook]
---
# T-006 Template library pipeline

## Goal
Infrastructure for a tested C++ template library based on the ICPC notebook, plus the task pipeline
used for every task from now on.

Requirements (from the user):
- Templates are **more flexible** than the notebook's (which minimized typing): generic types,
  lambdas for operations, self-contained (no contest macros), richer APIs.
- **Every template has tests**; every change must pass them (or change the test deliberately).
- Stress tests against brute force, plus judge links (`Verify:`) for manual checks.
- Insert templates from Neovim with a picker.
- Pipeline: read task → tests → implement → commit → merge to `main`.

## Plan
- [x] Pipeline documented: AGENTS.md, [[D-008 Task pipeline]], Task template checklist
- [x] Library format: `lib/<area>/<name>.cpp` with a metadata header ([[D-009 Template library format]])
- [x] Test harness: `tests/include/test.h` + `tests/run.py` (lint, standalone compile, stress tests, nvim tests)
- [x] Picker `<leader>rl` (`lua/util/lib.lua`) inserting templates and their dependencies
- [x] Tests for the picker (`tests/nvim/lib_test.lua`)
- [x] Guides updated (Keymaps, Competitive Programming, new Template Library guide)

## Log
- 2026-10-01: created. Split from [[T-005 Notebook-based templates]]; areas ported in follow-up tasks
  starting with [[T-007 Port dsa templates]].
- 2026-10-01: tests first: `tests/nvim/lib_test.lua` with fixtures (`tests/nvim/fixtures/lib`) failed
  (module missing), then `lua/util/lib.lua` made them pass. Runner checked on a throwaway template:
  catches a missing test, a forbidden `#define` and a failing `CHECK_EQ` (with a got/want diff);
  cached rebuilds take about 2 s. Picker checked end-to-end in a real session (`<leader>rl` →
  "top" → `<CR>` inserts Base then Top above `solve()`).

## Related
- Decisions: [[D-008 Task pipeline]], [[D-009 Template library format]]
- Guides: [[Template Library]]
