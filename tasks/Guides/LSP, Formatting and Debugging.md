---
updated: 2026-10-10
tags: [guide, cpp, go]
---
# LSP, Formatting and Debugging

## clangd
- Installed by Mason. For a lone `.cpp` it uses the fallback flags `-std=c++20 -DLOCAL`.
- In a real project, add `compile_commands.json` (e.g. `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON`)
  or a `compile_flags.txt` with one flag per line.
- No automatic `#include` insertion; no clang-tidy. See [[D-003 clangd flags for single-file C++]].
- `<leader>ch`: switch between source and header (clangd extension).

## Go
LazyVim's `lang.go` extra plus `test.core` (neotest), see [[T-027 Go tooling]]. Tools come from
Mason (`gopls`, `gofumpt`, `goimports`, `golangci-lint`, `delve`, `gomodifytags`, `impl`); the Go
toolchain itself is the system `go`.
- **LSP**: gopls with staticcheck, extra analyses (nilness, unusedparams, ...), inlay hints
  (`<leader>uh`) and code lenses (run test, `go mod tidy`, govulncheck: `<leader>cc`). Open the
  project at the module root (`go.mod`/`go.work`) so gopls sees the whole module.
- **Formatting** on save: `goimports` (adds/removes imports) then `gofumpt` (stricter gofmt).
- **Linting**: `golangci-lint` on save and on leaving Insert mode. It uses the project's
  `.golangci.yml` when there is one, otherwise its defaults.
- **Code actions** (`<leader>ca`): fill a struct, add/remove struct tags (gomodifytags), stub an
  interface (impl).
- **Run**: `<leader>rr` runs `go run .` for the file's package; `<leader>ri` asks for arguments.
- **Tests** (neotest + neotest-golang): `<leader>tr` nearest, `<leader>tt` file, `<leader>tT` all,
  `<leader>ts` summary; results show as signs and virtual text. `<leader>td` debugs a test.
- **Debugging**: delve through nvim-dap-go. `<leader>dc` offers *Debug* (the current file),
  *Debug Package*, *Debug (Arguments)*, *Debug test*, *Debug test (go.mod)* and *Attach*. See [[Keymaps]].

## Formatting
- Format on save is on. Toggle with `<leader>uf` (global) or `<leader>uF` (buffer).
- Go uses `goimports` + `gofumpt` (see [[#Go]]).
- C/C++ uses `clang-format`: the nearest `.clang-format` above the file, otherwise the config's
  `.clang-format` (LLVM style, 4 spaces). See [[D-002 clang-format with a fallback style file]].

## Debugging
1. Build with debug info: `<leader>rd` (or `g++ -g -O0 ...`).
2. Set a breakpoint with `<leader>db`, start with `<leader>dc`, choose the **codelldb** config,
   and enter the path to the binary.
3. `<leader>du` toggles the UI (scopes, stack, watches). See [[Keymaps]].
For programs that read stdin, use `<leader>da` (run with args) or temporarily read from a file
under `#ifdef LOCAL` (`freopen("in.txt", "r", stdin);`).

## Diagnostics
- `<leader>cd` line diagnostics, `]d`/`[d` to jump, `<leader>xx` list everything (Trouble).
- Compiler errors from `<leader>rc` go to the quickfix list: `<leader>xQ`, or `]q`/`[q` to jump.
