---
status: accepted
date: 2026-10-01
tags: [decision, ui]
---
# D-011 Font and Material icons

## Context
No Nerd Font was installed (Alacritty used the generic `monospace`), so the icons used across
LazyVim (file types, git, dashboard, diagnostics) were missing. The user wanted a better font and
Material icons.

## Decision
- Install **JetBrainsMono Nerd Font** (Nerd Fonts v3.5.1, official release) into
  `~/.local/share/fonts/JetBrainsMonoNerdFont/` and use it for all four styles in Alacritty.
- Keep **mini.icons** (LazyVim default): its glyphs are the Material Design Icons included in
  Nerd Fonts v3 (`nf-md-*`). Alternatives like nvim-material-icon (a web-devicons fork that needs
  bridging into LazyVim) or real-icons.nvim (needs the Kitty graphics protocol, which Alacritty
  lacks) add nothing here.

## Consequences
- Ligatures: none (Alacritty doesn't support them), so the font was chosen for legibility.
- Moving to another machine: install a Nerd Font v3+, or the icons show as boxes.

## Related
- Tasks: [[T-013 Editor aesthetics]]
