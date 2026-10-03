---
updated: 2026-10-03
tags: [guide, ui]
---
# UI and Colorscheme

| Element | Plugin | Where |
| ------- | ------ | ----- |
| Colorscheme | kanagawa.nvim (`dragon`, transparent, custom palette) | `lua/plugins/colorscheme.lua` |
| Dashboard ("BENDER VIM") | snacks.dashboard | `lua/plugins/ui.lua` |
| Statusline ("bubbles", no code context: the winbar has it) | lualine | `lua/plugins/ui.lua`, `options.lua` |
| Rainbow indent guides (scope guide not animated) | snacks.indent + `Rainbow*` highlights | `ui.lua` + `colorscheme.lua` |
| Winbar (path + symbols) | dropbar.nvim | `lua/plugins/ui.lua` |
| Color codes (`#aabbcc`) highlighted | mini.hipatterns extra | `lua/config/lazy.lua` |
| Underlined word under the cursor | `LspReference*` highlights | `colorscheme.lua` |
| Tabs / buffers | bufferline (LazyVim default) | none (LazyVim default) |
| Smooth scrolling | **off** (snacks.scroll), scrolling is instant | `ui.lua` |
| Rainbow brackets | rainbow-delimiters.nvim + `RainbowDelimiter*` highlights | `ui.lua` + `colorscheme.lua` |
| Syntax highlighting | Treesitter, started right after the first screen (Vim regex syntax until then) | `ui.lua` + `lua/util/perf.lua` |
| Sticky function/loop header | nvim-treesitter-context (LazyVim extra `ui.treesitter-context`) | `lua/config/lazy.lua` |
| Inline diagnostics (rounded bubbles) | tiny-inline-diagnostic.nvim (`modern` preset), default virtual text off | `ui.lua` |

## Font and icons
- Terminal font: **JetBrainsMono Nerd Font** (Nerd Fonts v3.5.1), installed in
  `~/.local/share/fonts/JetBrainsMonoNerdFont/` and set in `~/.config/alacritty/alacritty.toml`
  (backup: `alacritty.toml.bak`). See [[D-011 Font and Material icons]].
- Icons come from **mini.icons** (LazyVim default), which uses the Material Design Icons of
  Nerd Fonts (`nf-md-*`), so no extra icon plugin is needed.
- Alacritty doesn't render ligatures, whatever the font.
- Toggles: `<leader>ut` (sticky context), `<leader>uS` (smooth scroll, this
  session only).

## Performance
See [[T-014 Performance]], [[T-022 Snappier editor]], [[D-012 Performance budget]] and
[[D-018 Paint first, Treesitter after]].
- No smear cursor: its animation kept the screen moving ~90 ms after every jump (removed in T-022).
- The first file of each language opens with Vim's regex colors and switches to Treesitter's
  ~0.3 s later (C++); rainbow brackets and folds arrive at the same moment. Compiling the C++
  queries (~380 ms) happens once per session, so later C++ files get everything at once.
- Measure: `:Lazy profile`, or `nvim --startuptime /tmp/st.log file.cpp`. Real per-key latency needs
  a pty driver (method in [[T-022 Snappier editor]]).

## Changing colors
- Palette overrides: `colors.palette` in `colorscheme.lua`. The mapping from terminal colors is in
  [[D-004 Kanagawa Dragon with custom palette]].
- Single highlight groups: add them to `overrides` in `colorscheme.lua`.
- Floats are transparent like the editor ([[D-005 Transparent floats]]).
- Inspect a highlight under the cursor: `<leader>ui`. List all of them: `<leader>sH`.
