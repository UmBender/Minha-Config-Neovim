---
status: accepted
date: 2026-10-02
tags: [decision, cpp, notebook]
---
# D-017 Geometry point type

## Context
The notebook's geometry templates share `core.cpp`: `using Pt = complex<ll>` with
`#define xx real()` / `#define yy imag()`, free `dot` / `cross` / `perp`, and `sgn` with a global
`EPS`. Each template carries comments like `// ld: use sgn(...)`, `// ld: change ret type to ld`:
switching to floating coordinates means editing the inserted code by hand. `complex` of an integer
type is unspecified by the standard, and the library forbids `#define` in templates.

## Decision
- `geometry/point` defines `template <class T> struct Point { T x, y; }` (an aggregate:
  `P{1, 2}`), with operators, `<=>` (by x, then y), members `dot`, `cross`, `cross(a, b)` (around
  `*this`), `dist2`, `dist`, `angle`, `perp`, `unit`, `rotate`, an explicit conversion between
  coordinate types and stream operators. Every other geometry template `Requires: geometry/point`.
- `sgn(x)` is one function for both worlds: exact for integer `T`, `|x| <= EPS` is zero for floating
  `T` (`if constexpr`). `EPS = 1e-9` is a global `const long double`, the only knob to tune.
- All geometry functions are templates over `T`: integer `T` (`long long`) is exact for
  orientation tests, hulls, areas, closest pair, Minkowski, faces (coordinates up to ~1e9); the
  functions that build new points (line / circle intersections, tangents, cuts, enclosing circle)
  need a floating `T` and say so in their Usage.
- Lines are always given by two points; results that may not exist are reported (`lineInter`'s
  `{k, p}`, empty vectors) instead of being preconditions.

## Consequences
- The same inserted code serves `long long` and `long double` in one solution; no hand edits.
- `EPS` is a global name: a solution with its own `EPS` gets a redefinition error and must drop one.
- Members (`a.cross(b)`) instead of the notebook's free functions (`cross(a, b)`): slightly more
  typing, no clash with user helpers of the same name.

## Related
- Tasks: [[T-021 Port geometry templates]]
- Decisions: [[D-009 Template library format]]
