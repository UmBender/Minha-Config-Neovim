---
status: accepted
date: 2026-10-01
tags: [decision, cpp]
---
# D-003 clangd flags for single-file C++

## Context
Competitive programming files are lone `.cpp` files without `compile_commands.json`, using
`bits/stdc++.h` and `#ifdef LOCAL` debug code.

## Decision
- `fallbackFlags = { "-std=c++20", "-DLOCAL" }` so C++20 features work and `LOCAL` blocks are active.
- `--header-insertion=never`: no automatic `#include` lines.
- No `--clang-tidy` (too noisy for contest code).
- `--function-arg-placeholders=1`: clangd 23 requires the value; LazyVim's default only worked
  because the next flag was consumed as its value.

## Consequences
For real projects, a `compile_commands.json` or `compile_flags.txt` takes precedence over the fallback flags.

## Related
- Tasks: [[T-002 C++ competitive programming setup]]
