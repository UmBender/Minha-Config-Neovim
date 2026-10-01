---
status: todo
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook]
---
# T-016 Port strings templates

## Goal
Port `notebook/content/strings` (11 templates: aho-corasick, kmp, lcp, longest-common-substring,
lyndon, manacher, runs, suffix-array, suffix-automaton, suffix-tree, z-function) to `lib/strings/`
with tests, examples and docs, in the format in force after [[T-015 Template variant menu]]
(which changes how common uses are packaged, so it goes first).

## Plan
- [ ] Read each notebook file + `notebook/content/strings/main.tex`, write the per-template table
  (notebook -> library -> changes), like [[T-012 Port graph templates]]
- [ ] Tests (stress vs brute force) failing first, then templates + examples
- [ ] Mutation check, docs regenerated

## Pipeline
- [ ] Task read, requirements and plan written (status `doing`)
- [ ] Branch `task/T-016-port-strings`
- [ ] Tests written and failing for the right reason
- [ ] Implemented; `python3 tests/run.py` passes in full
- [ ] Vault updated (log, guides, decisions)
- [ ] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-01: created (after [[T-015 Template variant menu]]).

## Related
- Tasks: [[T-005 Notebook-based templates]]
