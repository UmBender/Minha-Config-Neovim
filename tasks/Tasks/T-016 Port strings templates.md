---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook]
---
# T-016 Port strings templates

## Goal
Port `notebook/content/strings` (11 templates: aho-corasick, kmp, lcp, longest-common-substring,
lyndon, manacher, runs, suffix-array, suffix-automaton, suffix-tree, z-function) to `lib/strings/`
with tests, examples and docs, in the format in force after [[T-015 Template variant menu]]
([[D-009 Template library format]], [[D-013 Template variants]]).

## Requirements
- One template per notebook file, header + test (edge cases + stress vs brute force) + example.
- Generic over the sequence type where it costs nothing (`string`, `vector<int>`, ...): functions
  take `const S &s` and only use `size()` / `operator[]` / `==` / `<`.
- Fixed alphabet automata (Aho-Corasick, suffix automaton) are parameterized by alphabet size and
  base character (`<26, 'a'>` by default), so digits, uppercase or small int alphabets work.
- Variants: none planned. These are single-purpose functions/structures without the
  "same structure, different operation" shape that motivated [[D-013 Template variants]].

## Plan

| Notebook | Library | Changes |
| -------- | ------- | ------- |
| kmp | `strings/kmp` | `prefixFunction(s)` (generic), `kmpMatches(text, pat)`: start of every occurrence |
| z-function | `strings/z-function` | `zFunction(s)` (generic, `z[0] = n`), `zMatches(text, pat)` |
| suffix-array | `strings/suffix-array` | `suffixArray(s)` generic (any comparable values), prefix doubling + counting sort |
| lcp | `strings/lcp` | `lcpArray(s, sa)` (Kasai), same format (`lcp[0] = 0`, `lcp[i] = lcp(sa[i-1], sa[i])`) |
| suffix-tree | `strings/suffix-tree` | `SuffixTree(s)` builds sa + lcp itself (`Requires` both); nodes `l, r, par, suf, depth, ch`; children in lexicographic order; no sentinel needed |
| suffix-automaton | `strings/suffix-automaton` | `SuffixAutomaton<A, Base>`: `extend`, `ord()`, `countEndpos()`, `find(pat)`, `occurrences(pat)`, `distinctSubstrings()`, `first` (end of first occurrence) |
| longest-common-substring | `strings/longest-common-substring` | returns positions in both strings + length, `Requires: strings/suffix-automaton` |
| manacher | `strings/manacher` | `Manacher` struct: `odd`, `even` (notebook's d1/d2), `isPalindrome(l, r)` O(1), `longest()` |
| aho-corasick | `strings/aho-corasick` | `AhoCorasick<A, Base>`: `add` returns pattern id, `end[id]`, `build`, `next`, `link`, `ord`, `out` (patterns ending here), `count(text)` per pattern |
| lyndon | `strings/lyndon` | `lyndon(s)` split points `{0, ..., n}` (generic), `minRotation(s)` |
| runs | `strings/runs` | `runs(s)` -> `{p, l, r}` sorted by `(p, l)`, `Requires: strings/z-function` |

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-016-port-strings`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Mutation check
- [x] Vault updated (log, guides, decisions), docs regenerated
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-01: created (after [[T-015 Template variant menu]]).
- 2026-10-01: started. Read the notebook files and `main.tex`, wrote the plan above.
- 2026-10-01: tests written (`tests/strings/*.cpp`, stress vs brute force, plus `vector<int>`
  inputs and custom alphabets) and failing (no templates yet).
- 2026-10-01: implemented the 11 templates and examples; full suite passes (85 templates).
  Notes:
  - `suffix-tree`: kept the notebook's construction from sa + lcp; every suffix ends at a node
    (`suf`), possibly an internal one when it is a prefix of a longer suffix, so no sentinel is
    needed. Split nodes are created after the child they split, so node ids are **not** in
    parent-first order (my first test assumed so; the test walks from the root instead).
  - `suffix-automaton`: added `first` (end of the first occurrence; clones copy it), so
    `longest-common-substring` returns positions in both strings, not only the string.
  - `aho-corasick`: `out` is propagated along suffix links in `build()`, so "how many patterns end
    here" is O(1) online; `count(text)` pushes hits up in reverse BFS order.
  - `lyndon`: added `minRotation` (Duval on `s + s`).
  - No variants (see Requirements).
- 2026-10-01: mutation check (one or more planted bugs per template). All caught except
  equivalent mutants: `lcp` decrementing `k` by 2 (still correct and linear) and an extra
  `break` in `lyndon`. Dropping `k` entirely in `lcp` (correct but O(n^2)) survived at first:
  added a 200000-character test so it times out -> caught.

## Related
- Tasks: [[T-005 Notebook-based templates]], [[T-012 Port graph templates]]
- Decisions: [[D-009 Template library format]], [[D-013 Template variants]]
