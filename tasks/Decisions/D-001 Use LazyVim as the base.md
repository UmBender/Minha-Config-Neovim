---
status: accepted
date: 2026-10-01
tags: [decision]
---
# D-001 Use LazyVim as the base

## Context
The old config (packer, lsp-zero v3, ~40 `after/plugin` files, several unused colorschemes) was hard
to maintain and partly broken.

## Decision
Use LazyVim with its defaults, and customize only through `lua/plugins/*.lua` specs. Keep the old
visual identity; rebuild everything else.

## Consequences
- Many defaults come from LazyVim (bufferline, snacks explorer/picker, blink.cmp, conform, trouble).
- Keymaps follow LazyVim (`<leader>s` = search, `<leader>q` = quit/session); the old
  `<leader>s` save is now `<C-s>`.
- Language support beyond C++ is added with `:LazyExtras` or an import in `lua/config/lazy.lua`.

## Related
- Tasks: [[T-001 Migrate to LazyVim]]
