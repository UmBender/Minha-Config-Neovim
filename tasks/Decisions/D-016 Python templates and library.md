---
status: accepted
date: 2026-10-01
tags: [decision, python, library]
---
# D-016 Python templates and library

## Context
The user uses Python for Project Euler, random generators, stress tests and brute forces
([[T-019 Python templates and library]]). They wanted templates for that and a library of small
helpers (random trees/DAGs/permutations, edge-case trees, number theory), not full algorithms.

## Decision
- **Insertable library, not an importable package**: `pylib/<area>/<name>.py` is pasted into the
  script with `<leader>rl`, like the C++ library ([[D-009 Template library format]]): same
  picker, `Requires`, `# Title:` .. `# End:` folds. Scripts stay standalone (a `gen.py` can be
  copied anywhere), and the workflow is the same in both languages. `lua/util/lib.lua` became one
  instance per language (`require("util.lib")` is C++, `.python` is Python).
- Templates are **self-contained**: stdlib imports inside the template (duplicate imports are
  harmless), no `if __name__`, no top-level side effects (lint). No variants. Titles are unique
  across both libraries (they name the docs pages, `tasks/Library/python/`).
- Generators use the **global `random`**, so `random.seed(argv[1])` in `gen.py` makes a test
  reproducible from its seed.
- **File templates are chosen by name** (`gen*.py`, `stress*.py`, `*brute*.py` / `sol*.py`,
  `euler*.py` / `p42.py`), not inserted into every new `.py`: Python is also used for unrelated
  scripts. `<leader>rn` picks one by hand.
- Tests are plain scripts (`tests/py/include/libtest.py`: `include`, `check`, `check_eq`), with no
  pytest dependency, run by `tests/run.py` under `-X dev -W error`.

## Consequences
- Same mental model for C++ and Python. A helper can't import another one: it declares
  `Requires` instead.
- No Python LSP is configured yet (`lazyvim.plugins.extras.lang.python` is not imported).

## Related
- Tasks: [[T-019 Python templates and library]]
- Decisions: [[D-009 Template library format]], [[D-014 Collapsible templates with native folds]]
