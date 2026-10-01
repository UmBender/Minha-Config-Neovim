---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task]
---
# T-001 Migrate to LazyVim

## Goal
Replace the old packer + lsp-zero config with LazyVim, keeping the visual identity and
rebuilding everything else from scratch.

## Plan
- [x] Remove `after/plugin/*`, `lua/bender/*`, `TODO.md`, `shortcut.md` (kept in git history)
- [x] LazyVim bootstrap (`init.lua`, `lua/config/*`)
- [x] Keep the visuals: dashboard header, bubbles lualine, rainbow indent, winbar, cursor word underline, color highlighting
- [x] Install plugins and verify a clean startup

## Log
- 2026-10-01: done in commit `5cf9593`. Old visuals ported: dashboard → snacks.dashboard,
  ibl → snacks.indent, winbar.nvim → dropbar.nvim, nvim-colorizer → mini.hipatterns extra,
  nvim-cursorline → `LspReference*` underline.
- Replaced by LazyVim defaults: barbar → bufferline, nvim-tree → snacks explorer, telescope → snacks picker.
- Dropped language setups (Zig, Rust, PHP, Elixir, LaTeX, Vue...). Re-add with `:LazyExtras` when needed.

## Related
- Decisions: [[D-001 Use LazyVim as the base]]
- Guides: [[Keymaps]], [[UI and Colorscheme]]
