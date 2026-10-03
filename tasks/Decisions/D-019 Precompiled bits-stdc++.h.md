---
status: accepted
date: 2026-10-03
tags: [decision, cpp, performance]
---
# D-019 Precompiled bits-stdc++.h

## Context
A compile of `templates/cp.cpp` took ~2 s (release) / ~2.8 s (debug) with GCC 16, almost all of it
parsing `bits/stdc++.h`. Solutions must stay single-file and portable (the judge compiles them as is).

## Decision
`lua/util/cp.lua` builds a GCC precompiled header of the **system** `bits/stdc++.h` per flag set, in
`stdpath("cache")/pch/<key>/bits/stdc++.h.gch`, and every compile (`<leader>rc/rr/rd`, CompetiTest)
adds `-I <key dir> -Winvalid-pch`. g++ finds the `.gch` before the real header; sources are untouched.

- `<key>` = sha256 of the flags + `g++ --version` + header path and mtime: a compiler/header update
  or a flag change gets a new PCH (no stale one), and other keys are pruned after a build (~150 MB each).
- Built in the background: release when a C++ buffer opens (after the first screen,
  `util.perf.when_ready`), debug on the first debug compile. One build per key; written to a temp
  file and renamed, so a compile never sees a half-written PCH.
- A missing PCH dir is harmless, and a PCH that doesn't match is ignored by g++ (falls back to the
  header) with a `-Winvalid-pch` warning, so the worst case is the old speed.
- CompetiTest's `compile_command` is now built from `util.cp.flags` (`opts` function, evaluated when
  the plugin loads, not at startup), so the two flag lists can't drift.

## Consequences
- Compile ~2 s -> ~0.6 s (release), ~2.8 s -> ~0.8 s (debug). See [[T-023 Precompiled header]].
- ~300 MB of cache with both PCHs; one ~6 s background g++ per GCC update / flag change.
- Not used by `tests/run.py` (library tests); it could be added there the same way if needed.
- clang/clangd don't use the GCC PCH (they are unaffected).

## Related
- Tasks: [[T-023 Precompiled header]]
- Decisions: [[D-018 Paint first, Treesitter after]]
- Guides: [[Competitive Programming]]
