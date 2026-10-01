---
status: accepted
date: 2026-10-01
tags: [decision, process]
---
# D-008 Task pipeline

## Context
The template work is large and must stay correct over many changes. The user asked for a fixed
pipeline: read the task → implement the tests → implement the task → commit → merge to `main`.

## Decision
Every task follows the pipeline in `AGENTS.md` (*Task pipeline*):
1. Task note (`doing`) with goal, requirements and checklist.
2. Branch `task/T-NNN-<slug>`.
3. Tests first; see them fail.
4. Implement until the **full** suite (`python3 tests/run.py`) passes. Changing an existing test
   is allowed only deliberately, with the reason logged in the task note.
5. Update the vault (task log, guides, decisions).
6. Commit on the branch.
7. Fast-forward merge into `main`, delete the branch. No push unless asked.

## Consequences
- Commit + merge are pre-authorized for pipeline tasks. This refines [[D-006 Git workflow]]
  (branch + merge), which still applies to work outside the pipeline.
- `main` always passes the full test suite.

## Related
- Tasks: [[T-006 Template library pipeline]]
