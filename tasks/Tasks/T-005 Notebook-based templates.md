---
status: todo
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook]
---
# T-005 Notebook-based templates

## Goal
Use the ICPC notebook (`notebook/` → `~/gempro-notebook`) as the base for templates in this config.
**Waiting for the user's detailed spec.**

## Plan
- [ ] Get the spec from the user
- [ ] Break it into steps here

## Context
- Contest template: `notebook/content/contest/template.cpp` (differs from `templates/cp.cpp`)
- ~100 templates in `notebook/content/{dsa,graph,math,strings,geometry,tree}/*.cpp`; reusable
  part after `// begin template //`; descriptions in each area's `main.tex` (Portuguese)
- Contest shell helpers: `notebook/content/contest/.bashrc` (`c`, `cs`, `gen`, `rr`, `chk`)

## Log
- 2026-10-01: created. Symlink fixed (it pointed to a relative path that didn't exist) and
  ignored in git.

## Related
- Decisions: [[D-007 Notebook is a read-only, untracked source]]
- Guides: [[Competitive Programming]]
