---
status: done
created: 2026-10-03
updated: 2026-10-03
tags: [task, lua, tests]
---
# T-024 Lua format check

## Goal
`stylua` is now installed. Make `tests/run.py` check that every Lua file matches `stylua.toml`
(`stylua --check .`), and format the files that drifted.

## Requirements
- New `lua` stage in `tests/run.py`, run with the nvim tests (no filter, or `nvim` / `lua` filter).
- Without `stylua` on `PATH` the stage is skipped with a note, not failed (the suite still runs on a
  machine without it).
- Keep `stylua.toml` as is (LazyVim's own settings); the code follows stylua's default output.
- Full `python3 tests/run.py` green.

## Plan
- [x] Stage in `tests/run.py` (fails now: 6 files drift)
- [x] `stylua .` on the drifting files
- [x] Guide: [[Template Library]] "Running the tests"

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-024-lua-format-check`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-03: created. `stylua --check .` reports 6 files: `after/ftplugin/{cpp,python}.lua`,
  `lua/plugins/ui.lua`, `tests/nvim/{lib,py,perf}_test.lua` (one-line `function() ... end` bodies).
  Considered `collapse_simple_statement = "FunctionOnly"` to keep the one-liners, but it rewrites
  far more code (most files use the expanded form), so the config stays and those files get formatted.

- 2026-10-03: `lua` stage added to `tests/run.py` (failed on the 6 files), `stylua .` formatted them,
  full `python3 tests/run.py` green. Guide [[Template Library]] updated.

## Related
- Decisions:
- Guides: [[Template Library]]
