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
| Smooth cursor (smear trail) | smear-cursor.nvim (LazyVim extra `ui.smear-cursor`) | `lua/config/lazy.lua` |
| Smooth scrolling | snacks.scroll (LazyVim default) | none |
| Rainbow brackets | rainbow-delimiters.nvim + `RainbowDelimiter*` highlights | `ui.lua` + `colorscheme.lua` |
| Sticky function/loop header | nvim-treesitter-context (LazyVim extra `ui.treesitter-context`) | `lua/config/lazy.lua` |
| Inline diagnostics (rounded bubbles) | tiny-inline-diagnostic.nvim (`modern` preset), default virtual text off | `ui.lua` |

## Font and icons
- Terminal font: **JetBrainsMono Nerd Font** (Nerd Fonts v3.5.1), installed in
  `~/.local/share/fonts/JetBrainsMonoNerdFont/` and set in `~/.config/alacritty/alacritty.toml`
  (backup: `alacritty.toml.bak`). See [[D-011 Font and Material icons]].
- Icons come from **mini.icons** (LazyVim default), which uses the Material Design Icons of
  Nerd Fonts (`nf-md-*`), so no extra icon plugin is needed.
- Alacritty doesn't render ligatures, whatever the font.
- Toggles: `:SmearCursorToggle`, `<leader>ut` (sticky context).

## Changing colors
- Palette overrides: `colors.palette` in `colorscheme.lua`. The mapping from terminal colors is in
  [[D-004 Kanagawa Dragon with custom palette]].
- Single highlight groups: add them to `overrides` in `colorscheme.lua`.
- Floats are transparent like the editor ([[D-005 Transparent floats]]).
- Inspect a highlight under the cursor: `<leader>ui`. List all of them: `<leader>sH`.
