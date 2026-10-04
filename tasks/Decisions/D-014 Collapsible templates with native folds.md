---
status: superseded (fold layout, see D-020)
date: 2026-10-01
tags: [decision]
---
# D-014 Collapsible templates with native folds

## Context
Inserted library templates make solution files long, and most of the time only their behavior
matters. They should collapse to one line and open on demand, also after reopening the file.

## Decision
- `<leader>rl` appends `// End: <Title>` after each inserted template (user's choice). A template
  block is `// Title: X` .. `// End: X`; a Title without its End is left alone.
- Native folds, no plugin (user's choice over nvim-ufo, which replaces folds with manual ones and
  adds a dependency). `lua/util/fold.lua` provides the cpp foldexpr: template blocks are level-1
  folds; everything else is LazyVim's treesitter foldexpr, shifted one level inside a template.
- `after/ftplugin/cpp.lua` sets that foldexpr and collapses all templates when a file opens;
  inserted templates collapse right away; `<leader>rf` toggles all of them; `za`/`zo`/`zc` work.
- `foldtext` stays `""` (LazyVim): a closed template shows its own highlighted `// Title: X`
  line. Neovim 0.12 draws neither eol nor ephemeral inline virtual text on a closed fold, and a
  custom `foldtext` would apply to every fold and lose the highlighted look.

## Consequences
- One extra comment line per template in the submitted code.
- Files with templates pasted by hand (no End line) don't fold them; adding the End line by hand
  does.

## Related
- Tasks: [[T-017 Collapsible templates]]
- Superseded in part by: [[D-020 Only templates open collapsed]]
