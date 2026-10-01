---
status: accepted
date: 2026-10-01
tags: [decision, cpp, library]
---
# D-013 Template variants

## Context
[[D-010 Template presets, examples and generated docs]] packed the common uses of a structure into
its file as presets (`minSegtree(a)`, `RangeAddSum<T>`, ...). Inserting a segment tree then pasted
all of them, and the generic code was harder to read. The user asked for a menu of **variants**
instead ([[T-015 Template variant menu]]): normal, educational, and several common uses, each
common use being short specialized code, not the generic template plus a wrapper.

## Decision
- **Layout**: variants sit next to the template, the variant name is the file's second extension:
  - `lib/<area>/<name>.cpp`: **normal**, only the generic structure (default template arguments
    are allowed, they are part of the generic API).
  - `lib/<area>/<name>.edu.cpp`: **educational**, the same code and API, commented to explain how
    and why it works.
  - `lib/<area>/<name>.<use>.cpp`: **common** uses (`segtree.sum`, `lazy-segtree.add-sum`, ...),
    standalone specialized code. A variant's id is `<area>/<name>.<use>`.
  No `// Variant:` header key: the file name already says it, one fewer thing to keep in sync.
- **Headers**: every variant has the full header (`Title`, `Description`, `Usage`, `Complexity`).
  The educational variant keeps the **same Title** as the normal one: it defines the same names, so
  insertion treats them as the same template (inserting one when the other is present is a no-op).
  Common variants have their own Title (e.g. `Segment tree (sum)`).
- **Tests**:
  - educational: the runner compiles the normal template's test unchanged, with
    `#include "<area>/<name>.cpp"` redirected to the `.edu.cpp` (an include overlay directory).
    Reported as `<area>/<name>.edu`. It has no test or example of its own.
  - common: `tests/<area>/<name>.<use>.cpp` (edge cases + stress vs brute force) and
    `examples/<area>/<name>.<use>.cpp`, required by lint like any template.
- **Docs**: one page per structure (`tasks/Library/<area>/<Title>.md`) with a **Variants**
  section (one subsection per common variant: description, usage, complexity, example). The
  catalog keeps one row per structure and lists its variants.
- **Presets are gone**: the `// Presets` header key is rejected by lint; existing presets moved
  to common variants (or into the `Usage:` text when they were only default arguments or methods).
- **Picker** (`<leader>rl`): one entry per structure (search also matches variant names and
  descriptions). A structure with variants opens a second menu (normal, educational, common uses
  in alphabetical order); one without variants inserts directly.
- **Scope**: variants only where they help: segment trees, lazy segment tree, Fenwick, DSU, sparse
  table get an educational version; common variants where a structure had presets or an obvious
  common use.

## Consequences
- Inserting a common variant pastes only that short code.
- Adding a common variant = file + test + example, like a template. Adding an educational variant
  is just the file (the existing test covers it).
- Supersedes the *presets* part of [[D-010 Template presets, examples and generated docs]]; its
  examples and generated docs parts stay.

## Related
- Tasks: [[T-015 Template variant menu]]
- Decisions: [[D-009 Template library format]], [[D-010 Template presets, examples and generated docs]]
