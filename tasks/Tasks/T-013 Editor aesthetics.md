---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, ui]
---
# T-013 Editor aesthetics

## Goal
More aesthetic editor: smooth ("soft") cursor and similar plugins, a good font for Alacritty, and an
icon font, focusing on Material icons.

## Plan
- [x] Research plugins, fonts and icons
- [x] User picked: JetBrains Mono NF; smear cursor, rainbow brackets, sticky context, pretty diagnostics;
  permission to install the font and edit Alacritty
- [x] Font installed + Alacritty updated (backup `alacritty.toml.bak`)
- [x] Plugins + highlights from the Kanagawa palette
- [x] Verified in a real session (all 4 plugins load, `RainbowDelimiter*` colors, virtual text off)

## Pipeline
- [x] Task read; branch `task/T-013-aesthetics`
- [x] Tests: no automated test (plugins load only with a UI); verified in a pty session instead
- [x] Full suite passes; vault updated ([[UI and Colorscheme]], [[D-011 Font and Material icons]])
- [x] Committed and merged

## Log
- 2026-10-01: created. Found: no Nerd Font installed (Alacritty uses `monospace`), so icons
  most likely render as boxes.
- 2026-10-01: research. smear-cursor.nvim (also a LazyVim extra), real-icons.nvim (needs the
  Kitty graphics protocol, not Alacritty), nvim-material-icon (fork of web-devicons). mini.icons
  already uses Material Design Icons (`nf-md`), so installing a Nerd Font is the fix.
