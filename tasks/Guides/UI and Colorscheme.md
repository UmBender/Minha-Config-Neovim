---
updated: 2026-10-01
tags: [guide, ui]
---
# UI and Colorscheme

| Element | Plugin | Where |
| ------- | ------ | ----- |
| Colorscheme | kanagawa.nvim (`dragon`, transparent, custom palette) | `lua/plugins/colorscheme.lua` |
| Dashboard ("BENDER VIM") | snacks.dashboard | `lua/plugins/ui.lua` |
| Statusline ("bubbles") | lualine | `lua/plugins/ui.lua` |
| Rainbow indent guides | snacks.indent + `Rainbow*` highlights | `ui.lua` + `colorscheme.lua` |
| Winbar (path + symbols) | dropbar.nvim | `lua/plugins/ui.lua` |
| Color codes (`#aabbcc`) highlighted | mini.hipatterns extra | `lua/config/lazy.lua` |
| Underlined word under the cursor | `LspReference*` highlights | `colorscheme.lua` |
| Tabs / buffers | bufferline (LazyVim default) | none (LazyVim default) |

## Changing colors
- Palette overrides: `colors.palette` in `colorscheme.lua`. The mapping from terminal colors is in
  [[D-004 Kanagawa Dragon with custom palette]].
- Single highlight groups: add them to `overrides` in `colorscheme.lua`.
- Floats are transparent like the editor ([[D-005 Transparent floats]]).
- Inspect a highlight under the cursor: `<leader>ui`. List all of them: `<leader>sH`.
