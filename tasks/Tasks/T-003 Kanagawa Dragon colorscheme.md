---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, ui]
---
# T-003 Kanagawa Dragon colorscheme

## Goal
Replace gruvbox with Kanagawa Dragon using the user's higher-contrast terminal palette, and keep the
rainbow indent guides with the new colors.

## Plan
- [x] Diff the user's palette against stock Kanagawa Dragon (13 accents + 2 extended colors changed)
- [x] Map each terminal color to its kanagawa.nvim palette entry and override it
- [x] Rainbow indent groups use palette colors
- [x] Remove gruvbox; disable tokyonight/catppuccin bundled by LazyVim

## Log
- 2026-10-01: done in commit `6d1dd32`. The palette `.toml` was only a reference and is not tracked.

## Related
- Decisions: [[D-004 Kanagawa Dragon with custom palette]]
- Guides: [[UI and Colorscheme]]
