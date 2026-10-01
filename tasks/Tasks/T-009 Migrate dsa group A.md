---
status: todo
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook, agent]
---
# T-009 Migrate dsa group A

## Goal
Apply [[D-010 Template presets, examples and generated docs]] to: `fenwick-tree, max-add-segtree, sparse-table, dsu, dsu-rollback, offline-deletions, mo-queries, suffix-max` (all in `lib/dsa`).
Runs in a background agent, in its own git worktree.

## Plan
- [ ] For each template: presets for its most common uses (tests first), example, remove `// Pending:`

## Pipeline
- [ ] Task read, requirements and plan written (status `doing`)
- [ ] Branch `task/T-009-...`
- [ ] Tests written and failing for the right reason
- [ ] Implemented; `python3 tests/run.py` passes in full
- [ ] Vault updated (log)
- [ ] Committed (merged into `main` by the main session)

## Log
- 2026-10-01: created by the main session.
