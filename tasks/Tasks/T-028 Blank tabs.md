---
status: done
created: 2026-10-10
updated: 2026-10-10
tags: [task, ui, go]
---
# T-028 Blank tabs

## Goal
In Go files every tab showed as `>` and the code looked unformatted (not like `go fmt`). Draw tabs
blank and make sure Go formatting matches `gofmt`.

## Plan
- [x] Find the cause: LazyVim sets `list`, Neovim's default `listchars` is `tab:> ,trail:-,nbsp:+`,
  and Go indents with real tabs (the Go ftplugin sets `noexpandtab`).
- [x] Check formatting: save already runs `goimports` + `gofumpt` (a gofmt superset).
- [x] `listchars` with `tab:  ` in `lua/config/options.lua`, keeping the `trail`/`nbsp` markers.
- [x] Test `tests/nvim/options_test.lua`.
- [x] Guides: UI and Colorscheme, LSP/formatting.

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-028-blank-tabs`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-10: created. Reproduced in a pty on a scratch module: `expandtab=false`, `listchars` the
  default `tab:> ,...`; typing a tab-indented line and `:w` ran goimports + gofumpt and the file
  came out exactly gofmt-style (4-space indent rewritten to tabs). So formatting worked; the `>`
  markers on every tab made it look wrong.
- 2026-10-10: `tests/nvim/options_test.lua` failed (`got "> "`), then passed after setting
  `listchars = { tab = "  ", trail = "-", nbsp = "+" }`. Re-checked in a pty. Full suite passes.
  Indentation stays visible through the rainbow indent guides (snacks.indent).

## Related
- Tasks: [[T-027 Go tooling]]
- Guides: [[UI and Colorscheme]], [[LSP, Formatting and Debugging]]
