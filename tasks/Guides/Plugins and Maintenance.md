---
updated: 2026-10-01
tags: [guide]
---
# Plugins and Maintenance

| Command | What it does |
| ------- | ------------ |
| `:Lazy` (`<leader>l`) | Plugin manager: `U` update, `S` sync, `X` clean, `R` restore from the lockfile |
| `:LazyExtras` | Enable or disable LazyVim extras (other languages, tools) |
| `:Mason` | LSP servers, formatters, debuggers (clangd, clang-format, codelldb, ...) |
| `:LazyHealth` / `:checkhealth` | Diagnose problems |
| `:TSUpdate` | Update treesitter parsers |

## Updating safely
1. `:Lazy update`, then test the C++ workflow ([[Competitive Programming]]).
2. If something breaks, `:Lazy restore` goes back to the versions in `lazy-lock.json`.
3. Commit `lazy-lock.json` once everything works.

## Adding a language
`:LazyExtras` → pick `lang.<name>` → restart. Or add `{ import = "lazyvim.plugins.extras.lang.<name>" }`
to `lua/config/lazy.lua` (preferred here, so it is versioned).

## Optional tools
- `fd`: faster file finding.
- `lazygit`: `<leader>gg` git UI.
- Competitive Companion browser extension (see [[Competitive Programming]]).
