---
status: done
created: 2026-10-02
updated: 2026-10-02
tags: [task, cpp, notebook]
---
# T-021 Port geometry templates

## Goal
Port `notebook/content/geometry` (14 files) to `lib/geometry/` with tests, examples and docs, in the
format in force ([[D-009 Template library format]], [[D-013 Template variants]]). Last area of
[[T-005 Notebook-based templates]].

## Requirements
- One template per notebook file, header + test (edge cases + stress vs brute force) + example.
- **Point type** ([[D-017 Geometry point type]]): the notebook uses `complex<ll>` + `#define xx/yy`
  and asks to hand-edit `ll` -> `ld` and `sgn` per problem. Here: `geometry/point` defines
  `template <class T> struct Point` (x, y, operators, `dot`, `cross`, `dist2`, ...) and
  `sgn(x)`, exact for integer `T` and `EPS`-tolerant for floating `T`. Every other geometry template
  `Requires: geometry/point` and is generic in `T`; the ones that build new points (intersections,
  circles, cuts) need a floating `T`.
- Lines are always given by two points (the notebook's `circLineInter` took a direction).
- Degenerate cases handled and documented instead of left to the caller where cheap (parallel
  lines, duplicates in closest pair / hull, coincident circles).
- Variants: none planned (single-purpose functions).

## Plan

| Notebook | Library | Changes |
| -------- | ------- | ------- |
| core | `geometry/point` | `Point<T>` struct instead of `complex<ll>`; `sgn`, `EPS`; `dot`, `cross`, `cross(a, b)` around `*this`, `dist2`, `dist`, `perp`, `rotate`, `angle`, `unit`, `<`/`==`, stream ops |
| line-intersection | `geometry/line-intersection` | `lineInter(a, b, c, d)` -> `{1, p}` / `{0, _}` parallel / `{-1, _}` same line (notebook: precondition) |
| segment-intersection | `geometry/segment-intersection` | `segInter`, `properInter` (exact on integers), plus `onSegment(p, a, b)` |
| polygon-area | `geometry/polygon-area` | `polyArea2(p)`: twice the signed area (name says it), any `T` |
| point-in-polygon | `geometry/point-in-polygon` | `inConvex(p, q)` O(log n) and `inPolygon(p, q)` O(n) for any simple polygon; both return `-1/0/1` (outside/border/inside) instead of a `border` flag |
| convex-hull | `geometry/convex-hull` | `convexHull(pts, collinear = false)` -> CCW indices; duplicates, n <= 2, all-collinear handled |
| closest-pair | `geometry/closest-pair` | `closestPair(pts)` -> pair of indices; duplicates handled inside |
| circle-line | `geometry/circle-line` | `circleLine(c, r, a, b)` (line through a, b) |
| circle-circle | `geometry/circle-circle` | `circleCircle(c1, r1, c2, r2)`; coincident -> empty (documented) |
| circle-tangents | `geometry/circle-tangents` | `circleTangents(c1, r1, c2, r2)`: distinct tangents only (no duplicates on touching circles, `r2 = 0` gives tangents from a point) |
| enclosing-circle | `geometry/enclosing-circle` | `enclosingCircle(pts)` -> `{center, r}`; own RNG (no global `rng`) |
| minkowski | `geometry/minkowski` | `minkowski(a, b)` any `T`; collinear vertices removed from the result |
| polygon-cut | `geometry/polygon-cut` | `polygonCut(p, a, b)` keeps the left side of a -> b; no `lineInter` dependency |
| planar-graph-faces | `geometry/planar-graph-faces` | `planarFaces(pts, g)` -> faces as vertex cycles (inner faces CCW, outer CW) |

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-021-port-geometry`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Mutation check
- [x] Vault updated (log, guides, decisions), docs regenerated
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-02: created and started (last area of [[T-005 Notebook-based templates]] after
  [[T-020 Port tree templates]]). Read the notebook files and `main.tex`, wrote the plan above.
- 2026-10-02: tests written (`tests/geometry/*.cpp`, 14 files) and failing (no templates). Brute
  forces: parametric segment/line intersection with exact integers, winding number (sum of angles)
  for point-in-polygon, O(n^3) hull by "every point left of the edge", O(n^2) closest pair,
  circumcircle over all pairs/triples, hull of all pairwise sums for Minkowski, Euler's formula +
  per-component area cancellation for planar faces, area additivity of both sides for cuts.
- 2026-10-02: implemented the 14 templates and examples; full suite passes (120 templates).
  Notes / differences from the notebook:
  - `Point<T>` aggregate instead of `complex<ll>` + macros; `sgn` exact on integers, `EPS` on
    floating types ([[D-017 Geometry point type]]).
  - `point-in-polygon`: two functions returning -1/0/1; `inConvex` also accepts collinear vertices
    (if `p[0]` is a corner) and n <= 2. Needed care: with collinear chains through `p[0]` the
    binary search must treat the ray `p[0] -> p[l]` differently for the first and last sides.
  - `minkowski`: the notebook compares edges by `cross` only, so opposite edges (cross 0) look
    parallel. That only happens with 2-vertex polygons (segments), which the notebook doesn't
    support; here edges are ordered by half-plane then cross, so a point or a segment works, and
    collinear vertices are removed from the result.
  - `convex-hull`: duplicates (lowest index kept), all-collinear input and n <= 2 handled.
  - `closest-pair`: returns indices, handles duplicates itself (the notebook's judge test did it
    outside).
  - `circle-tangents`: distinct lines only (touching circles and point + circle gave duplicates).
  - `circle-line`: line by two points (was point + direction).
  - `enclosing-circle`: own `mt19937` (the notebook used a global `rng`).
  - `planar-graph-faces`: `map` + sort as in the notebook, faces' orientation documented.
- 2026-10-02: mutation check, 38 planted bugs. Survivors and what was done:
  - `stable_sort -> sort` in the hull survived (tests too small to leave insertion sort): added a
    200-point duplicates test, now caught.
  - closest pair's `d++` (integer window margin) was redundant and hid the `<= p.y + d` bound:
    removed; both boundary mutants now caught.
  - Minkowski's `dot >= 0` in the collinear-removal step was dead code (results never backtrack once
    opposite edges are ordered): removed.
  - Equivalent mutants left: `(c-a)x(d-a) == (c-a)x(d-c)` in `lineInter`; erasing window points with
    `dx^2 == best` in closest pair (they can't be strictly closer).

## Related
- Tasks: [[T-005 Notebook-based templates]], [[T-020 Port tree templates]]
- Decisions: [[D-009 Template library format]], [[D-013 Template variants]], [[D-017 Geometry point type]]
