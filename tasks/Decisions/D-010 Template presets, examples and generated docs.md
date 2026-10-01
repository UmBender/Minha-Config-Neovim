---
status: accepted
date: 2026-10-01
tags: [decision, cpp, notebook]
---
# D-010 Template presets, examples and generated docs

## Context
The user asked for (1) an example for every template showing how to use it, in the docs, and
(2) every generic template callable directly in its most common use (e.g. a lazy segment tree for
range add + range sum) without writing the lambdas.

## Decision
- **Presets** live in the template file: default template arguments, factory functions
  (`minSegtree(a)`) or wrapper structs with plain values (`RangeAddSum<T>`). Listed in a
  `// Presets:` header key, tested in the template's test.
- **Examples**: `examples/<area>/<name>.cpp`, a full program with `Problem:`, `Input:` and `Output:`
  in its header. The runner compiles it with the test flags and checks the output, so the docs
  never show broken code.
- **Docs are generated**: `tests/run.py --write-docs` writes one vault page per template
  (`tasks/Library/<area>/<Title>.md`: description, usage, presets, complexity, example + I/O,
  judge links) and the catalog `tasks/Library/Library.md`. The normal run fails when they're stale.
- Migration of existing templates uses a temporary `// Pending:` header key, removed per template.
- Page names come from titles, so titles can't contain `\ / : # ^ [ ] |` (lint).

## Consequences
- Adding a template = template + test + example. Docs come for free.
- The catalog that used to be hand-written in [[Template Library]] is now generated ([[Library]]).

## Related
- Tasks: [[T-008 Template presets and examples]]
- Supersedes nothing; extends [[D-009 Template library format]].
