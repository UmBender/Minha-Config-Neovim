---
status: done
created: 2026-10-02
updated: 2026-10-02
tags: [task, cpp, notebook]
---
# T-020 Port tree templates

## Goal
Port `notebook/content/tree` (6 templates: centroid-decomp, hld, lca, tree-diameter, tree-matching,
virtual-tree) to `lib/tree/` with tests, examples and docs, in the format in force
([[D-009 Template library format]], [[D-013 Template variants]]).

## Requirements
- One template per notebook file, header + test (edge cases + stress vs brute force) + example.
- Input is an undirected adjacency list `vector<vector<int>>` (each edge in both lists), 0-indexed.
- **No recursion**: the notebook recurses (lambdas / member DFS), which overflows the stack on deep
  trees (a path of 2e5 vertices, worse under ASan). Every traversal here is iterative; tests include a
  long path.
- Variants: none planned (single-purpose structures, no "same structure, different op" shape).

## Plan

| Notebook | Library | Changes |
| -------- | ------- | ------- |
| lca | `tree/lca` | `LCA(g, root)`: DFS-order + sparse table (O(1) query, n-1 entries instead of the Euler tour's 2n), no separate `SparseTable` dependency; exposes `par`, `depth`, `tin`, `tout` (subtree = `[tin, tout)`), plus `dist`, `isAncestor` |
| hld | `tree/hld` | `HLD(g, root)`: `pos`, `head`, `par`, `depth`, `size`; `lca`, `dist`, `kthAncestor`, `jump` (k-th vertex on a path), `subtree(v)` -> `[l, r)`; `segments(u, v, edges)` unordered half-open ranges (commutative ops); `path(u, v, edges)` ordered `{l, r, up}` for non-commutative ops; `edges = true` drops the LCA (values on edges stored at the child) |
| tree-diameter | `tree/tree-diameter` | `treeDiameter(g)` -> path vertices; weighted overload `treeDiameter(wg)` -> `{length, path}` (non-negative weights) |
| tree-matching | `tree/tree-matching` | `treeMatching(g)` -> mate or `-1`; forests (every component) |
| centroid-decomp | `tree/centroid-decomp` | `CentroidTree ct(g)`: `par` (centroid tree parent, `-1` at roots) and `level` (depth in the centroid tree, `<= log2 n`); forests |
| virtual-tree | `tree/virtual-tree` | `virtualTree(lca, vs)` -> `{vertices, edges}`: vertices in DFS order (`[0]` is the root), edges `(parent, child)`; input untouched, duplicates OK; `Requires: tree/lca` |

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-020-port-tree`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Mutation check
- [x] Vault updated (log, guides, decisions), docs regenerated
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-02: created and started (next area of [[T-005 Notebook-based templates]] after
  [[T-018 Port math templates]]). Read the notebook files and `main.tex`, wrote the plan above.
- 2026-10-02: tests written (`tests/tree/*.cpp`: BFS parents + climbing LCA, brute paths, bitmask
  maximum matching, all-pairs distances, centroid-tree checked by re-deriving the components) and
  failing (no templates). Added `test::rndTree(n, span)` to `test.h` (random shapes: path, shallow,
  in between; shuffled labels and neighbor order).
- 2026-10-02: implemented the 6 templates and examples; full suite passes (106 templates).
  Notes / differences from the notebook:
  - All traversals iterative (the notebook recursed); each test runs a 2e5-vertex path.
  - `lca`: DFS-order trick (min `tin[par]` over `(tin[u], tin[v]]`) instead of the Euler tour, and
    no external `SparseTable`; exposes `tin`/`tout`/`depth`/`par` (used by `virtual-tree`).
  - `hld`: the notebook's `path` gave inclusive `(tin, tin)` pairs whose direction was encoded by
    `l > r`; now half-open `{l, r, up}`, plus unordered `segments`, the `edges` flag, `subtree`,
    `kthAncestor`, `jump`. Built without copying/erasing the adjacency list.
  - `tree-diameter`: weighted overload (any non-negative `T`), returns `{length, path}`.
  - `tree-matching`, `centroid-decomp`: forests supported (the notebook only handled vertex 0's
    component); centroid tree also gives `level` and a top-down `order`.
  - `virtual-tree`: no longer sorts the caller's vector in place; returns the vertex list too.
  - No variants (see Requirements).
- 2026-10-02: mutation check, 30 planted bugs, 28 caught; the 2 survivors are equivalent mutants
  (`jump` at `k == du` returns the LCA from either side; HLD may jump either chain on equal head
  depths).

## Related
- Tasks: [[T-005 Notebook-based templates]], [[T-018 Port math templates]]
- Decisions: [[D-009 Template library format]], [[D-013 Template variants]]
