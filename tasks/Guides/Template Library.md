---
updated: 2026-10-01
tags: [guide, cpp, notebook]
---
# Template Library

Tested C++ templates in `lib/<area>/<name>.cpp`, based on the ICPC notebook but more flexible
(see [[D-009 Template library format]]).

## Using a template
1. In a `.cpp` buffer press `<leader>rl` and fuzzy-search by name, area or description.
   The preview shows the whole file, including the `Usage:` block.
2. `<CR>` inserts it **above `void solve()`** (or `int main()`, or below the cursor), together with
   any templates it `Requires`. Templates already in the file are skipped.
3. Read the `Usage:` comment at the top of the inserted code. Most templates have **presets** for
   the common case (e.g. `auto seg = minSegtree(a);`, `RangeAddSum<long long> seg(a);`).
   Each template's page in [[Library]] has a full example.

Conventions in every template: 0-indexed, half-open ranges `[l, r)`, generic types, operations
passed as lambdas.

## Running the tests

```sh
python3 tests/run.py            # everything: lint, standalone compile, C++ tests, nvim tests
python3 tests/run.py segtree    # only ids containing "segtree"
python3 tests/run.py nvim       # only the Neovim helper tests
```

All tests run under `-Werror`, ASan/UBSan and `_GLIBCXX_DEBUG`. Builds are cached, so only changed
tests recompile.

## Adding or changing a template
Follow [[D-008 Task pipeline]]: test first, then code, then the full suite.
1. `tests/<area>/<name>.cpp`: edge cases + random stress against a brute force (presets too).
2. `lib/<area>/<name>.cpp`: header (`Title`, `Description`, `Usage`, `Complexity`, optional
   `Presets`/`Verify`/`Requires`) + code + presets. No `#include`/`#define`/`assert`.
3. `examples/<area>/<name>.cpp`: a small complete solution with `Problem:`, `Input:`, `Output:`.
4. `python3 tests/run.py --write-docs` regenerates the docs pages; then `python3 tests/run.py`
   must pass in full. See [[D-010 Template presets, examples and generated docs]].

## Catalog
Generated: see **[[Library]]**, with one page per template (usage, presets, a runnable example and
judge links).

> [!warning] `assert` with GCC 16
> `bits/stdc++.h` no longer includes `<cassert>`. If a solution uses `assert`, add `#include <cassert>`.
