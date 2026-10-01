---
status: doing
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook]
---
# T-005 Notebook-based templates

## Goal
Use the ICPC notebook (`notebook/` → `~/gempro-notebook`) as the base for templates in this config.
**Waiting for the user's detailed spec.**

## Plan
- [x] Get the spec from the user
- [x] Break it into tasks: [[T-006 Template library pipeline]] (infrastructure), then one task per
  area, starting with [[T-007 Port dsa templates]] as a pilot for review
- [ ] graph, math, strings, geometry, tree (after the pilot is reviewed)

## Spec (2026-10-01)
- The notebook minimizes typing; the library must be **more flexible**: generic types, lambdas for
  ops, self-contained, richer APIs.
- Every template has tests (stress against brute force + judge links); every change must pass them.
- Pipeline per task: read → tests → implement → commit → merge to `main` ([[D-008 Task pipeline]]).
- Insertion through a picker.

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
