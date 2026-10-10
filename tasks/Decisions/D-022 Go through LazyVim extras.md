---
status: accepted
date: 2026-10-10
tags: [decision, go]
---
# D-022 Go through LazyVim extras

## Context
Go is needed for regular module-based projects (a job application), not for competitive
programming ([[T-027 Go tooling]]). LazyVim ships a maintained `lang.go` extra, and test running
needs `test.core` (neotest), which this config didn't use before.

## Decision
- Import `lazyvim.plugins.extras.lang.go` and `lazyvim.plugins.extras.test.core` in
  `lua/config/lazy.lua`, with no overrides: gopls settings, goimports + gofumpt, golangci-lint,
  delve/nvim-dap-go and neotest-golang are used as upstream configures them.
- The only custom part is `after/ftplugin/go.lua`: `<leader>rr`/`<leader>ri` run `go run .` from
  the file's directory (the whole package, not the single file, since Go packages span files).
  Tests stay on LazyVim's `<leader>t`, debugging on `<leader>d`.
- Adding the plugins pinned only the three new ones in `lazy-lock.json`; the other plugins were
  kept at their locked commits (no incidental updates in this change).

## Consequences
- `<leader>t` (neotest) now exists in every buffer, but neotest only loads when a key is used; the
  only adapter is Go's.
- golangci-lint runs on save in any Go buffer; a project's `.golangci.yml` controls it.
- gofumpt is stricter than gofmt: a project that enforces plain gofmt may see extra diffs. Turn it
  off per project if needed (`gopls.gofumpt` and the conform formatter list).

## Related
- Tasks: [[T-027 Go tooling]]
- Guides: [[LSP, Formatting and Debugging]], [[Keymaps]]
