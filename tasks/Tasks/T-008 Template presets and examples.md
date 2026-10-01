---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook]
---
# T-008 Template presets and examples

## Goal
New standard for every template ([[D-010 Template presets, examples and generated docs]]):
presets for the most common use, a runnable example, generated docs pages.

## Plan
- [x] Runner: examples stage (compile + run + compare output), docs generation/check, lint
- [x] Pilot on `dsa/segtree` (`sumSegtree`/`minSegtree`/`maxSegtree`) and `dsa/lazy-segtree`
  (`RangeAddSum`, `RangeAddMin/Max`, `RangeAssignSum/Min/Max`), preset tests first
- [x] `// Pending:` marker for the 23 dsa templates migrated by agents ([[T-009 Migrate dsa group A]],
  [[T-010 Migrate dsa group B]], [[T-011 Migrate dsa group C]])
- [x] AGENTS.md, decision, guide

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-008-examples-presets`
- [x] Tests written and failing for the right reason (preset tests: undeclared presets)
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-01: the examples stage caught a wrong expected output in the lazy-segtree example (the
  affine update also sees the earlier `+10`). Titles with `/` produced subdirectories for docs
  pages, so two titles were renamed and lint now rejects those characters.
