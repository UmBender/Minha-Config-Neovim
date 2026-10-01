---
status: accepted
date: 2026-10-01
tags: [decision, notebook]
---
# D-007 Notebook is a read-only, untracked source

## Context
`notebook` is a symlink to `~/gempro-notebook`, the ICPC team library, which lives in its own repo.

## Decision
- The symlink is ignored (`/notebook` in `.gitignore`) and never committed.
- The config never edits files inside the notebook; it only reads them as a base for templates.
- The symlink target is absolute (`/home/bender/gempro-notebook`). The original relative target did
  not resolve.

## Related
- Tasks: [[T-005 Notebook-based templates]]
