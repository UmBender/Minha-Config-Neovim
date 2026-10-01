---
status: accepted
date: 2026-10-01
tags: [decision, ui]
---
# D-004 Kanagawa Dragon with custom palette

## Context
The user's terminal uses a higher-contrast Kanagawa Dragon palette. Neovim should match it.

## Decision
Use `rebelot/kanagawa.nvim` (`dragon`, transparent) and override palette entries via
`colors.palette`, mapping each terminal color to the entry kanagawa uses for it:

| Terminal color | Palette entry | Value |
| -------------- | ------------- | ----- |
| red | dragonRed | `#d16961` |
| green | dragonGreen2 | `#8aa86e` |
| yellow | dragonYellow | `#ceb680` |
| blue | dragonBlue2 | `#7fa8bc` |
| magenta | dragonPink | `#aa88ac` |
| cyan | dragonAqua | `#82b0ab` |
| white | oldWhite | `#d0c58b` |
| bright red | waveRed | `#ec6070` |
| bright green | dragonGreen | `#7bb57b` |
| bright yellow | carpYellow | `#ecc57e` |
| bright blue | springBlue | `#75b8d3` |
| bright magenta | springViolet1 | `#8e7eb5` |
| bright cyan | waveAqua2 | `#6db5a7` |
| extended 1 | dragonOrange | `#c28f6f` |
| extended 2 | dragonOrange2 | `#c4886f` |

## Consequences
Syntax colors follow the terminal palette. If the terminal palette changes, update
`lua/plugins/colorscheme.lua` by hand (the palette file is not tracked).

## Related
- Tasks: [[T-003 Kanagawa Dragon colorscheme]]
