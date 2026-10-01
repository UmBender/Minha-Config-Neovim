---
status: accepted
date: 2026-10-01
tags: [decision, cpp, notebook]
---
# D-009 Template library format

## Context
The notebook's templates minimize typing (shared macros, function-pointer template parameters,
hardcoded types) and are mostly untested. In the editor, typing isn't the bottleneck, so the user
wants flexible, tested templates.

## Decision
- One template per file: `lib/<area>/<name>.cpp`; the areas mirror the notebook (`dsa`, `graph`, ...).
- The file **is** what gets inserted, so the tested code is exactly what ends up in a solution.
- Metadata header with `Title`, `Description`, `Usage`, `Complexity` (required), `Verify`, `Requires`
  (optional). `Requires` lists other templates, which the picker inserts first.
- Self-contained (no includes, macros or pragmas), generic, lambdas for operations, richer APIs.
- Tests at `tests/<area>/<name>.cpp`: edge cases + stress against brute force, built with
  `-Werror`, sanitizers and the debug STL. `Verify:` links keep the judge problems for manual checks.
- Insertion with the picker `<leader>rl` (`lua/util/lib.lua`): dependencies first, skips templates
  already in the file (matched by `// Title:`), inserts above `void solve` / `int main` / the cursor.
- Templates that are redundant once generic (e.g. a "simple" variant of a generic one) are merged,
  and the merge is logged in the porting task.

## Consequences
- Adding a template = header + code + test. `tests/run.py` refuses a template without a test.
- Snippets are not generated from the library; `snippets/cpp.json` stays for tiny idioms.

## Related
- Tasks: [[T-006 Template library pipeline]]
