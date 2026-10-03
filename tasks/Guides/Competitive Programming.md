---
updated: 2026-10-03
tags: [guide, cpp]
---
# Competitive Programming

## Quick start
1. `nvim a.cpp` opens a new file with `templates/cp.cpp` already in it, cursor inside `solve()`.
2. Write the solution; `<C-s>` saves and formats.
3. `<leader>rr` compiles and runs it in a floating terminal; type or paste the input there.
   `q` (normal mode) closes the terminal.
4. If it crashes or behaves oddly, `<leader>rd` reruns with AddressSanitizer, UBSan and the
   checked STL (`_GLIBCXX_DEBUG`).

## Compile flags

| Build | Flags |
| ----- | ----- |
| normal (`rr`, `rc`, CompetiTest) | `-std=c++20 -O2 -Wall -Wextra -Wshadow -DLOCAL` |
| debug (`rd`) | `-std=c++20 -g -O0 -Wall -Wextra -Wshadow -DLOCAL -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address,undefined` |

The binary is written next to the source (`a.cpp` → `a`). Flags are defined in `lua/util/cp.lua`;
CompetiTest (`lua/plugins/cpp.lua`) takes them from there.

## Precompiled header
`bits/stdc++.h` is precompiled once per flag set, so a compile takes ~0.6 s instead of ~2 s
(see [[D-019 Precompiled bits-stdc++.h]]). Nothing to do by hand:

- The normal one is built in the background (~6 s) when you open a C++ file; the debug one on the
  first `<leader>rd`. Until it exists, compiles just take the usual time.
- It lives in `~/.cache/nvim/pch/<key>/bits/stdc++.h.gch` (~150 MB each) and is used through
  `-I <dir> -Winvalid-pch`. The key covers the flags, `g++ --version` and the header, so a GCC
  update or a flag change builds a new one and the old ones are deleted.
- If g++ ever ignores it, `-Winvalid-pch` puts a warning on line 1 in quickfix saying why.
  To force a rebuild: `rm -rf ~/.cache/nvim/pch`.
- Your source is untouched, so it compiles the same on the judge.

## Debug output
`LOCAL` is defined locally (and for clangd), never on the judge:

```cpp
dbg(n, v[i]);   // prints "[n, v[i]] = 5 3" to stderr locally, compiles to nothing on the judge
```

## Test cases (CompetiTest)
- `<leader>ra` adds a test case (input + expected output), `<leader>rt` runs all of them.
- The UI (`<leader>ru`) shows each case with status, time, and a diff of expected vs. actual output.
  Inside it: `R` reruns the case, `<C-r>` reruns all, `i` shows the input, `d` toggles the diff,
  `q` closes.
- Time limit per case: 5 s (`maximum_time` in `lua/plugins/cpp.lua`).

## Competitive Companion
Install the [Competitive Companion](https://github.com/jmerle/competitive-companion) browser
extension. Then:
1. `<leader>rp` (problem), `<leader>rP` (contest) or `<leader>rR` (tests for the current file).
2. Click the extension's green **+** on the problem page.
3. The `.cpp` file is created from the template, with test cases already loaded.

## Snippets

| Prefix | Expands to |
| ------ | ---------- |
| `fori` | `for (int i = 0; i < n; i++) { }` |
| `all` | `v.begin(), v.end()` |
| `vread` | read a `vector<int>` of size `n` |
| `yesno` | `cout << (ok ? "YES" : "NO") << '\n';` |

Snippets live in `snippets/cpp.json` (VS Code format).

## Template library
`<leader>rl` inserts a tested template (segment trees, DSU, FFT, ...) above `solve()`.
See [[Template Library]].

## Python
Brute forces, generators and stress tests in Python (and Project Euler): see [[Python]].
