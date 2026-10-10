---
status: done
created: 2026-10-10
updated: 2026-10-10
tags: [task, go]
---
# T-027 Go tooling

## Goal
Set up Go for a job application project (regular Go modules, not competitive programming):
LSP, formatting, linting, debugging and tests, following LazyVim idioms.

## Plan
- [x] Import the LazyVim extras `lang.go` (gopls with gofumpt/staticcheck, goimports + gofumpt via
  conform, golangci-lint via nvim-lint, delve + nvim-dap-go, gomod/gowork/gosum Treesitter) and
  `test.core` (neotest; `lang.go` adds the neotest-golang adapter).
- [x] Install the Mason tools (gopls, gofumpt, goimports, golangci-lint, delve) and parsers.
- [x] Buffer-local Go keymaps in `after/ftplugin/go.lua`: `<leader>rr` runs the file's package
  (`go run .`) in a float, `<leader>ri` with arguments, like Python.
- [x] Test `tests/nvim/go_test.lua`: extras imported, run command, buffer-local keymaps.
- [x] Verify in a pty: gopls attaches, format on save, golangci-lint, a test runs with neotest.
- [x] Guides: LSP/formatting/debugging, Keymaps.

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-027-go-tooling`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-10: created. Go 1.26.8 is installed system-wide; no Go tools in Mason yet.
- 2026-10-10: `tests/nvim/go_test.lua` (extras imported, `cp.go_run_cmd`, buffer-local `<leader>rr`/`ri`)
  failed for the right reasons, then passed. Imported `lang.go` + `test.core`; added
  `cp.go_run_cmd`/`cp.run_go` and `after/ftplugin/go.lua`. `Lazy! sync` also bumped unrelated
  plugins, so the lockfile was reset to the old pins plus neotest, neotest-golang and nvim-dap-go,
  then `Lazy! restore`. Mason: gopls, gofumpt, goimports, golangci-lint 2.14, delve, gomodifytags,
  impl. Verified in a pty on a scratch module: gopls attaches, Go parser present, save runs
  goimports + gofumpt (dropped an unused import, reformatted), lint config `golangcilint`, neotest
  ran `TestAdd` (1 passed); golangci-lint flags an unused func from the CLI. Full suite passes.
  Decision [[D-022 Go through LazyVim extras]]; guides updated.

## Related
- Decisions: [[D-022 Go through LazyVim extras]]
- Guides: [[LSP, Formatting and Debugging]], [[Keymaps]]
