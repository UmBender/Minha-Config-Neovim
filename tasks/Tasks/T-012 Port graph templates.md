---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook]
---
# T-012 Port graph templates

## Goal
Port `notebook/content/graph` (18 templates) to `lib/graph/` with the new standard
([[D-009 Template library format]] + [[D-010 Template presets, examples and generated docs]]).

## Plan

| Notebook | Library | Changes |
| -------- | ------- | ------- |
| scc | `graph/scc` | `SCC` struct: components in topological order, `components()`, `condensation()`, iterative |
| 2-sat | `graph/2-sat` | `TwoSat`: clauses, implications, `mustBe`, `atMostOne` (aux vars), `Requires: graph/scc` |
| bfs01 | `graph/bfs01` | `BFS01<T>` (0/1 weights) + preset `BFS` (unweighted), `dist`/`par`/`path`, multi-source |
| dijkstra | `graph/dijkstra` | `Dijkstra<T>`: any weight type, `path`, multi-source |
| bridges | `graph/bridges` | `TwoEdgeCC` from an edge list (parallel edges OK): `comp`, `isBridge`, `bridges()`, `tree()` |
| block-cut-tree | `graph/block-cut-tree` | `BlockCutTree`: `isCut`, `blocks`, `tree` |
| dinic | `graph/dinic` | `Dinic<T = long long>`: edge ids, `flowOn(id)`, `minCut()` |
| mcf | `graph/mcf` | `MinCostFlow<Cap, Cost>`: negative costs (no negative cycles), edge ids, `flowOn(id)` |
| hopcroft-karp | `graph/hopcroft-karp` | `HopcroftKarp`: matching, `vertexCover()` (König) |
| matching | `graph/matching` | general matching (blossom): `generalMatching(g)` |
| matching-size | `graph/matching-size` | randomized Tutte rank, self-contained (own elimination mod p) |
| hungarian | `graph/hungarian` | generic `T`, n <= m, `hungarianMax` preset |
| directed-mst | `graph/directed-mst` | `directedMST(n, root, edges)` returns cost + chosen edge per vertex |
| dominator-tree | `graph/dominator-tree` | `dominatorTree(g, s)` |
| eulerian-walk | `graph/eulerian-walk` | `eulerianPath(n, edges, directed, cycle)` with automatic start, vertices + edge ids |
| functional-forest | `graph/functional-forest` | `FunctionalGraph`: cycle id/position, depth, entry vertex, `jump(v, k)` with k up to 1e18 |
| havel-hakimi | `graph/havel-hakimi` | returns `nullopt` if not graphical; fixes the notebook's `rem(u)` bug |
| st-numbering | `graph/st-numbering` | returns `{}` when G + st isn't biconnected |

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-012-port-graph`
- [x] Tests written (`tests/graph/*.cpp`, stress vs brute force) and failing (no templates yet)
- [x] Implemented (`lib/graph/*.cpp`) + examples (`examples/graph/*.cpp`); `python3 tests/run.py` passes in full
- [x] Mutation check (see log)
- [x] Docs regenerated (`tasks/Library/graph/`, catalog), vault updated
- [x] Committed and merged into `main`

## Log
- 2026-10-01: created.
- 2026-10-01: notebook bugs found while reading: `havel-hakimi` writes `int u = head[cur], rem(u);`,
  which declares a new `int rem` instead of calling the lambda, so the vertex is never removed from
  its bucket. `matching-size` depends on `gauss`, `mod` and `rng` from elsewhere.
- 2026-10-01: session resumed after `/clear` (tests were already written). Implemented all 18
  templates and their examples; all iterative where recursion depth could reach n (SCC, bridges,
  block-cut tree, dominator tree, st-numbering, Hierholzer). Directed MST is O(nm) contraction
  with edge recovery (simpler than the skew-heap version; fine for ICPC sizes).
- 2026-10-01: **test changed deliberately**: `tests/graph/mcf.cpp` made costs negative only on
  `u < v` edges, which still allows negative cycles (`u -> v` at -9, `v -> u` at +3). SSP can't
  handle negative cycles (and the brute force counts circulations), so the test hung. Costs are
  now `base + h[u] - h[v]` with `base >= 0`: negative edges, but every cycle costs >= 0.
- 2026-10-01: mutation check (one planted bug per template). Caught: all except
  - `bfs01` 0-edges pushed to the back and `dijkstra` processing stale entries: equivalent
    mutants (still correct, only slower), accepted.
  - `dinic` without residual updates: added a fixed case that needs flow cancellation -> caught.
  - `matching-size` with a symmetric instead of skew matrix: random dense graphs can't tell;
    added two disjoint triangles (symmetric rank 6, true answer 2) -> caught.
  - `matching` marking only one side of a blossom survived even 30k sparse stress runs (close
    to equivalent on small graphs); removing blossom contraction or the re-queueing of blossom
    vertices is caught, so blossoms are covered.
