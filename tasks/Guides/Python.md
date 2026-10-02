---
updated: 2026-10-01
tags: [guide, python]
---
# Python

Python is for Project Euler, random test generators, stress tests and brute forces that check
C++ solutions. Keymaps: [[Keymaps#Competitive programming (Python buffers, `<leader>r`)]].
Design: [[D-016 Python templates and library]].

## File templates (`templates/py/`)
A new `.py` file gets a template **only when its name says which one**. Other Python files start
empty. `<leader>rn` inserts a template by hand.

| File name | Template | What it is |
| --------- | -------- | ---------- |
| `sol*.py`, `*brute*.py` | `sol` | stdin solution: token reader `ni()` / `nl(k)` / `ns()`, `solve()`, multi-test loop |
| `gen*.py` | `gen` | random test generator; `python3 gen.py SEED` (same seed, same test) |
| `stress*.py` | `stress` | stress test: gen vs sol vs brute |
| `euler*.py`, `p42.py`, `pe42.py` | `euler` | Project Euler: `solve(n)`, assert on the small case, timing; `42` fills the header and link |

## Stress testing a C++ solution
1. `sol.cpp`: your solution. `brute.py`: the slow but obviously right one (from the `sol`
   template). `gen.py`: a random small test (use the library: `<leader>rl` → `gen/tree`, ...).
2. `stress.py`: check the config lines at the top: `COMPILE` (g++ run once, `[]` to skip), `GEN`,
   `SOL`, `BRUTE`, `TIMEOUT`. For floats or several valid answers, edit `accept(inp, out, ans)`
   (by default the outputs must match token by token).
3. `python3 stress.py 1000` (or `<leader>ri` → `1000`). It stops at the first difference, crash or
   timeout, shows the input and both outputs, and saves the input to `stress_input.txt`.
   `python3 stress.py 1000 5000` starts from seed 5000.
4. Rerun one case: `python3 gen.py 17 | ./sol`.

## Library (`pylib/`)
`<leader>rl` in a Python buffer inserts a tested helper above `def solve` (or `def gen`,
`def main`, `if __name__`), with what it `Requires`, folded like the C++ ones. Each helper brings
its own imports. Catalog: [[Library#python/gen]] and below.

- `gen/rand`: arrays, distinct values, permutations, strings, `rand_partition` (split the sum of
  `n` over tests), subsets, `fmt` / `fmt_edges` (1-indexed output)
- `gen/tree`: `rand_tree(n, kind)` with kinds `random` (uniform), `path`, `star`, `caterpillar`,
  `binary`, `broom`, `spider`, `double-star`, `deep`, `shallow`; `edge_case_trees(n)` gives one of
  each; `relabel`, `to_parents`, `all_trees(n)`
- `gen/graph`: random simple / connected / directed graphs, `rand_dag`, bipartite, functional,
  `weighted`, `is_dag`, `is_connected`
- `gen/exhaustive`: every small array, permutation, subset, string, partition, composition,
  graph, DAG (for checking a brute force or a formula on all small inputs)
- `nt/primes`, `nt/divisors`, `nt/modular`, `nt/digits`, `nt/cont-frac` (Pell), `nt/prime-count`
  (pi(n) and sum of primes in O(n^(3/4))), `nt/pythagorean`, `misc/matrix` (matrix power, `fib`,
  `lin_rec`)

Use builtins where they exist: `pow(a, -1, m)`, `math.comb`, `math.isqrt`, `math.lcm`,
`functools.cache`.

## Tests
`python3 tests/run.py py` runs only the Python part: lint (stdlib-only imports, no top-level side
effects, unique titles), each helper pasted alone into a script, `tests/py/<area>/<name>.py`
(randomized checks against brute force), the examples, and the end-to-end template tests
(`tests/py/templates/`). Adding a helper works like a C++ template ([[Template Library]]):
`pylib/<area>/<name>.py` with a `# Title:` header, a test, and `examples/py/<area>/<name>.py` that
uses `include("<area>/<name>")`, then `python3 tests/run.py --write-docs`.
