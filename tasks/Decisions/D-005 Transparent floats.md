---
status: accepted
date: 2026-10-01
tags: [decision, ui]
---
# D-005 Transparent floats

## Context
The snacks explorer is built from floating windows, so it used Kanagawa's float colors
(bg `#0d0c0c`, fg `oldWhite`) and looked like a different theme from the transparent editor.

## Decision
Set kanagawa `colors.theme.all.ui.float = { fg = "#c5c9c5", bg = "none", bg_border = "none" }` and give
`WinSeparator` a visible fg (`dragonBlack5`).

## Consequences
All floats (explorer, pickers, hover docs) share the editor look; borders keep them readable.
To scope it to the explorer only, override `SnacksPickerList`/`SnacksNormal` instead.

## Related
- Tasks: [[T-004 Explorer and float colors]]
