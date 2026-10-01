---
status: accepted
date: 2026-10-01
tags: [decision, cpp]
---
# D-002 clang-format with a fallback style file

## Context
clangd's `--fallback-style` only accepts style **names**; an inline `{BasedOnStyle: LLVM, IndentWidth: 4}`
is silently ignored, and LLVM style uses 2-space indents.

## Decision
Format C/C++ with conform + `clang-format` (installed by Mason). If a `.clang-format` exists above
the file, use it (`--style=file`); otherwise use this config's `.clang-format` (LLVM, 4 spaces,
short ifs/loops on one line, 100 columns).

## Consequences
- Format on save works for single files without any project setup.
- To change the default style, edit `.clang-format` at the config root.

## Related
- Tasks: [[T-002 C++ competitive programming setup]]
