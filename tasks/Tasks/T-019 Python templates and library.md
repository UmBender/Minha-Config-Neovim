---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, python]
---
# T-019 Python templates and library

## Goal
The user writes Python for Project Euler, for random test generators and stress tests, and for
brute-force solutions that check their C++ submissions. Add Python file templates for those jobs
and a small tested **Python library** of helpers. It should hold handy building blocks (random
DAGs, trees, permutations, edge-case trees, number-theory functions), not full algorithms.

## Requirements
- **File templates** (`templates/py/`):
  - `sol.py`: stdin solution / brute force (fast input, `solve()`, multi-test loop).
  - `gen.py`: random test generator, seed from `argv[1]` (so a stress loop is reproducible).
  - `stress.py`: runs `gen` / `sol` / `brute` for seeds 1..N, compares outputs (token-wise, or with
    a custom checker), stops at the first mismatch, shows the input and both outputs, and keeps
    the failing input in a file. Commands for C++ binaries or Python scripts.
  - `euler.py`: Project Euler: `solve(n)` + asserts against the statement's small example + timing.
- New `*.py` files get a template **only when the name says which one**: `gen*.py`, `stress*.py`,
  `brute*.py` / `sol*.py`, `euler*.py` / `p<N>.py` / `pe<N>.py`. Other Python files stay empty (Python is also used
  for unrelated scripts). `<leader>rn` picks a template by hand.
- **Library** `pylib/<area>/<name>.py`, inserted with `<leader>rl` like the C++ one (same picker,
  same `Title`..`End` folds, `Requires`). Same header format with `#` comments. Self-contained:
  stdlib imports only, inside the template (duplicate imports are harmless in Python). No variants.
- Each library file has a test `tests/py/<area>/<name>.py` (edge cases + randomized checks against
  a brute force), and an example `examples/py/<area>/<name>.py` (`Problem`/`Input`/`Output`) that
  the runner executes. Generated docs pages in `tasks/Library/python/`.
- Python keymaps in `after/ftplugin/python.lua` under `<leader>r`: run, template, library, folds,
  CompetiTest.

## Plan
- `lua/util/lib.lua`: one library instance per language (`cpp`: `lib/`, `//`, `.cpp`;
  `python`: `pylib/`, `#`, `.py`), `require("util.lib")` stays the C++ one.
- `lua/util/fold.lua`: `# Title:` .. `# End:` folds too.
- `lua/util/cp.lua`: Python templates (`py_template_for(name)`, `insert_py_template`), `run_py`.
- `tests/run.py`: Python stages (lint, standalone exec, tests, examples, docs), Python template
  tests (`tests/py/templates/`), `tests/py/include/libtest.py` (`include("gen/tree")`, `check_eq`).
- Library contents:

| Id | Contents |
| -- | -------- |
| `gen/rand` | seeding from argv, `rand_array`, `rand_distinct`, `rand_perm`, `rand_string`, `rand_partition`, `rand_subset`, `fmt_edges` |
| `gen/tree` | `rand_tree(n, kind)`: random (Prüfer), path, star, caterpillar, binary, broom, spider, double-star, deep, shallow; `edge_case_trees(n)`, `relabel`, `to_parents`, `all_trees(n)` |
| `gen/graph` | `rand_graph(n, m, connected, directed, ...)`, `rand_dag(n, m, connected)`, `rand_bipartite`, `rand_functional`, `weighted`, `is_dag` |
| `gen/exhaustive` | small inputs for brute checks: `all_arrays`, `all_perms`, `all_subsets`, `all_strings`, `all_partitions`, `all_compositions`, `all_graphs` |
| `nt/primes` | `sieve`, `primes_upto`, `is_prime` (deterministic Miller-Rabin), `factor` (Pollard rho, `Counter`), `spf_sieve` |
| `nt/divisors` | `divisors`, `num_divisors`, `sigma`, `phi`, `mobius`, `phi_sieve`, `mobius_sieve`, `divisor_count_sieve` |
| `nt/modular` | `inv_mod`, `ext_gcd`, `crt`, `Binom` (factorials mod p), `lcm_list` |
| `nt/digits` | `digits`, `from_digits`, `digit_sum`, `is_palindrome`, `iroot`, `is_square`, `num_len` |
| `nt/cont-frac` | `sqrt_cf`, `convergents`, `pell` |
| `nt/prime-count` | `prime_pi(n)` / `prime_sum(n)` (Lucy), O(n^(3/4)) |
| `nt/pythagorean` | `triples(limit)` primitive and all triples |
| `misc/matrix` | `mat_mul`, `mat_pow` (optionally mod), `fib(n, mod)` (fast doubling) |

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-019-python-cp`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-01: created. Design choices (see [[D-016 Python templates and library]]): insertable
  library (same workflow as C++) rather than an importable package; templates picked by file
  name, not on every new `.py`.
- 2026-10-01: tests first: `tests/py/<area>/<name>.py` (random checks against brute force, e.g.
  Cayley's count for `all_trees`, Prüfer uniformity on 4 vertices, permutation brute force for DAG
  acyclicity, trial division, enumeration of residues for `crt`), `tests/py/templates/templates.py`
  (sol/gen/euler run; stress finds a planted bug, keeps the failing input, reports crashes and
  compile errors), `tests/nvim/py_test.lua`. All failed (no files yet).
- 2026-10-01: implemented. `lib.lua` is now one instance per language; `fold.lua` also pairs
  `# Title:` / `# End:`; `cp.lua` got `py_template_for`, `insert_py_template` (`{{N}}` = number in
  the file name), `pick_py_template`, `run_py`; BufNewFile autocmd for `*.py`; CompetiTest runs
  Python with `python3` (its `run_command` is keyed by filetype, `python`). `tests/run.py` has a
  Python stage (lint via `ast`, standalone exec, tests, examples, docs in `tasks/Library/python/`).
  First version of the `factor` stress multiplied two ~2^61 primes (Pollard needs ~2^30 steps):
  the test now has at most one huge prime factor.
- 2026-10-01: mutation check, 36 planted bugs. Two survive and are equivalent: the dense/sparse
  switch in `rand_graph` (speed only) and `seen >= n - 1` in `all_dags` (a cycle has >= 2
  vertices). One more survived at first (`rand_functional(loops=False)` checked one sample): fixed.
  Checked in a real session (pty): `p10.py` gets the Euler template, cursor in `solve`, keymaps,
  library insert + folds, the file runs.
- Follow-up idea (not done): Python LSP (`lazyvim.plugins.extras.lang.python`), the user's call.

## Related
- Decisions: [[D-016 Python templates and library]], [[D-009 Template library format]]
- Guides: [[Python]], [[Keymaps]], [[Template Library]]
