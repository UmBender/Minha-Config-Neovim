---
updated: 2026-10-01
tags: [guide, cpp, notebook]
---
# Template Library

Tested C++ templates in `lib/<area>/<name>.cpp`, based on the ICPC notebook but more flexible
(see [[D-009 Template library format]]).

## Using a template
1. In a `.cpp` buffer press `<leader>rl` and fuzzy-search by name, area or description (the search
   also matches variant names, e.g. `add-min` finds the lazy segment tree).
   The preview shows the whole file, including the `Usage:` block. `+N variants` marks structures
   with variants.
2. `<CR>` on a structure **without variants** inserts it right away. A structure **with variants**
   opens a second menu ([[D-013 Template variants]]):
   - **normal**: the generic template (ops as lambdas, richest API)
   - **educational**: the same code and API, commented to explain how and why it works
   - **common uses**: short plain code for one job, e.g. segment tree `sum` / `min` / `max`,
     lazy segment tree `add-sum` / `assign-min` / ..., Fenwick `range`, sparse table `gcd`
3. The code goes **above `void solve()`** (or `int main()`, or below the cursor), together with any
   templates it `Requires`. Templates already in the file are skipped (normal and educational
   count as the same template).
4. Read the `Usage:` comment at the top of the inserted code. Each structure's page in [[Library]]
   has a full example per variant.

Conventions in every template: 0-indexed, half-open ranges `[l, r)`, generic types, operations
passed as lambdas (common-use variants are plain code instead).

## Running the tests

```sh
python3 tests/run.py            # everything: lint, standalone compile, C++ tests, nvim tests
python3 tests/run.py segtree    # only ids containing "segtree" (variants included)
python3 tests/run.py nvim       # only the Neovim helper tests
```

All tests run under `-Werror`, ASan/UBSan and `_GLIBCXX_DEBUG`. Builds are cached, so only changed
tests recompile. Educational variants show up as `<area>/<name>.edu`: the normal test compiled
against the `.edu.cpp`.

## Adding or changing a template
Follow [[D-008 Task pipeline]]: test first, then code, then the full suite.
1. `tests/<area>/<name>.cpp`: edge cases + random stress against a brute force.
2. `lib/<area>/<name>.cpp`: header (`Title`, `Description`, `Usage`, `Complexity`, optional
   `Verify`/`Requires`) + code. No `#include`/`#define`/`assert`.
3. `examples/<area>/<name>.cpp`: a small complete solution with `Problem:`, `Input:`, `Output:`.
4. `python3 tests/run.py --write-docs` regenerates the docs pages; then `python3 tests/run.py`
   must pass in full. See [[D-010 Template presets, examples and generated docs]].

### Variants
- Educational: `lib/<area>/<name>.edu.cpp`, same `Title` and API as the normal file, comments
  only. No test or example of its own (the normal test runs against it).
- Common use: `lib/<area>/<name>.<use>.cpp` with its own `Title`, plus
  `tests/<area>/<name>.<use>.cpp` and `examples/<area>/<name>.<use>.cpp`, like a template.
- The normal file keeps only the generic structure: the old `// Presets:` key is rejected by lint.

## Catalog
Generated: see **[[Library]]**, with one page per structure (usage, variants, a runnable example
per variant and judge links).

> [!warning] `assert` with GCC 16
> `bits/stdc++.h` no longer includes `<cassert>`. If a solution uses `assert`, add `#include <cassert>`.
