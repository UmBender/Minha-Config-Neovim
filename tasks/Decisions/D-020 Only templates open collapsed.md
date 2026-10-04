---
status: accepted
date: 2026-10-04
tags: [decision]
---
# D-020 Only templates open collapsed

## Context
With [[D-014 Collapsible templates with native folds]] each template was a level-1 fold, a sibling
of the Treesitter folds of the user's code. Neovim gives a new fold the state of the sibling fold
above it ("the new fold is closed if the fold above it is closed", `fold.c`). Templates are
collapsed in the ftplugin, before Treesitter parses ([[D-018 Paint first, Treesitter after]]), so
`solve()`, `main()` and every fold after a template were born collapsed when the file opened, and
again while typing whenever a fold disappeared and came back. The user only wants templates
collapsed: their own contest code must stay visible.

## Decision
Supersedes the fold layout of D-014 (the rest of it stands):
- A template is two folds on the same `// Title:` .. `// End:` lines: an outer level-1 wrapper that
  stays open and the inner level-2 fold, the one collapsed. Base (Treesitter) levels shift by two
  inside a template.
- The user's folds are siblings of the open wrapper, so they inherit "open".
- `util.fold.close` reopens a closed wrapper (`zc` twice) before collapsing the inner fold.

## Consequences
- Closing a template by hand closes the inner fold; a second `zc` also closes the wrapper, and a
  fold created right below it may then be born closed until the next `<leader>rf` / reopen.
- `zM` / `zR` and `foldlevel` count one extra level inside templates.

## Related
- Tasks: [[T-025 Fold only templates]], [[T-017 Collapsible templates]]
- Decisions: [[D-014 Collapsible templates with native folds]]
