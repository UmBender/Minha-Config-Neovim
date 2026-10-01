---
updated: 2026-10-01
tags: [guide, cpp]
---
# LSP, Formatting and Debugging

## clangd
- Installed by Mason. For a lone `.cpp` it uses the fallback flags `-std=c++20 -DLOCAL`.
- In a real project, add `compile_commands.json` (e.g. `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON`)
  or a `compile_flags.txt` with one flag per line.
- No automatic `#include` insertion; no clang-tidy. See [[D-003 clangd flags for single-file C++]].
- `<leader>ch`: switch between source and header (clangd extension).

## Formatting
- Format on save is on. Toggle with `<leader>uf` (global) or `<leader>uF` (buffer).
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
