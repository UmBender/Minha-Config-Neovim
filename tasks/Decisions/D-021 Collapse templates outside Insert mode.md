---
status: accepted
date: 2026-10-04
tags: [decision]
---
# D-021 Collapse templates outside Insert mode

## Context
The library picker (`<leader>rl`) confirms from its prompt, in Insert mode, so the template is
inserted and collapsed while Neovim is in Insert mode. Neovim doesn't update expr folds in Insert
mode, and the lines changed there don't reach the folds even after leaving it: only a full
recompute fixes them. At that point the fold of the user's code below the insertion point
(`solve()`) still spans the template lines, so collapsing the template collapsed `solve()` too
([[T-026 Insert keeps user folds]]).

## Decision
`util.fold.close` called in Insert/Replace mode waits for the next `ModeChanged` out of it, then
recomputes the folds of the buffer's template windows (resetting the window-local `foldexpr`)
and collapses the templates. Outside Insert mode it collapses right away, as before
([[D-020 Only templates open collapsed]]).

## Consequences
- A template inserted from the picker collapses once Insert mode is left (immediately in practice:
  the picker closes and returns to Normal mode).
- Anything else that edits a template buffer from Insert mode and then folds must do the same.

## Related
- Tasks: [[T-026 Insert keeps user folds]]
- Decisions: [[D-020 Only templates open collapsed]], [[D-014 Collapsible templates with native folds]]
