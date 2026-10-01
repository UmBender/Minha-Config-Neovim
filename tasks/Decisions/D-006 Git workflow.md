---
status: accepted
date: 2026-10-01
tags: [decision, process]
---
# D-006 Git workflow

## Decision
- Agents commit or push only when asked.
- Work is committed on a short-lived branch, then fast-forwarded into `main` when the user asks.
  Push only on request.
- Reference files dropped in temporarily (palette `.toml`, the `notebook` symlink) are never committed.
- `lazy-lock.json` is committed.

## Related
- [[D-007 Notebook is a read-only, untracked source]]
