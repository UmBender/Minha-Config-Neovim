---
status: done
created: 2026-10-03
updated: 2026-10-03
tags: [task, cpp, performance]
---
# T-023 Precompiled header

## Goal
Compiling a solution from `templates/cp.cpp` takes ~2 s, almost all of it parsing `bits/stdc++.h`.
Use a GCC precompiled header (PCH) so `<leader>rc/rr/rd` and CompetiTest compile faster, without
touching the source files (solutions stay portable: the judge never sees it).

## Requirements
- PCH of the system `bits/stdc++.h`, built with **exactly** the compile flags (release and debug
  each get their own), stored in `stdpath("cache")/pch/<key>/bits/stdc++.h.gch`, used via
  `-I <dir> -Winvalid-pch` (GCC picks the `.gch` up in place of the header).
- `<key>` hashes the flags + `g++ --version` + the header's path and mtime, so a GCC update or a
  flag change never uses a stale PCH; old keys are pruned.
- Built in the background: the release one when a C++ buffer opens, the debug one on the first
  debug compile. Until it exists compiles just take the normal time (the `-I` dir is harmless).
- Atomic write (temp file + rename), one build per key at a time.
- CompetiTest compiles with the same flags + PCH (its args derive from `util.cp`, keeping the two
  flag lists in sync by construction).
- Tests in `tests/nvim/cp_test.lua`; full `tests/run.py` green; guide updated.

## Plan
- [x] Measure (below)
- [x] Tests: key/dir, args, compile command, build + valid PCH (`-H` shows `!`), single build per key, prune, CompetiTest args
- [x] `lua/util/cp.lua`: `pch_dir`, `pch_args`, `build_pch`, `compile_cmd`; `compile` uses them
- [x] `after/ftplugin/cpp.lua`: start the release build; `lua/plugins/cpp.lua`: CompetiTest args from `util.cp`
- [x] Decision note, [[Competitive Programming]] guide

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-023-precompiled-header`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Measurements (2026-10-03, GCC 16.2.1, `templates/cp.cpp`)
| | no PCH | PCH |
|---|---|---|
| release (`-O2`) | 1950-2500 ms | 720-790 ms |
| debug (`_GLIBCXX_DEBUG` + sanitizers) | 2840 ms | 760 ms |

Building a PCH: 6-7 s; size 143 MB (release), 178 MB (debug). A PCH built with other flags is
ignored (GCC falls back to the real header; `-Winvalid-pch` says why), and a missing `-I` dir is
harmless.

## Changes
- `lua/util/cp.lua`: `pch_root`, `pch_dir(flags)`, `pch_args(flags)`, `build_pch(flags, cb)`,
  `prune_pch()`, `compile_cmd(file, exe, debug)`; `compile` uses the PCH and starts its build when missing.
- `after/ftplugin/cpp.lua`: builds the release PCH after the first screen (`util.perf.when_ready`).
- `lua/plugins/cpp.lua`: CompetiTest `opts` is a function; compile args = `util.cp.flags` + PCH args
  (the flag list is no longer duplicated; AGENTS.md updated).
- `tests/nvim/cp_test.lua` (new): key/dir, args, compile command, CompetiTest args, prune, real build
  (single build per key, no temp file left, `g++ -H` shows the `.gch` used). Mutations checked:
  no PCH args in `compile_cmd` (2 failures), no in-flight guard (1 failure).

## Results
Real session (pty, `nvim a.cpp`): the PCH appears ~5.5 s after opening; `<leader>rc` then reports
**545-586 ms** vs 1979 ms for the same g++ command without the PCH.

## Log
- 2026-10-03: created; measured (above). User asked to implement it.
- 2026-10-03: implemented (Changes above), full `tests/run.py` green, end-to-end check in a real
  session (Results). [[D-019 Precompiled bits-stdc++.h]], [[Competitive Programming]] guide updated.

## Related
- Decisions: [[D-019 Precompiled bits-stdc++.h]]
- Guides: [[Competitive Programming]]
