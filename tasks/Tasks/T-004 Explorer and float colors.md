---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, ui]
---
# T-004 Explorer and float colors

## Goal
The file explorer looked like a different colorscheme (dark `#0d0c0c` background, yellowish text)
and unused colorschemes were still in the lockfile.

## Plan
- [x] Make floats transparent with the editor foreground (`ui.float`)
- [x] Make `WinSeparator` visible on the transparent background
- [x] Remove tokyonight/catppuccin from `lazy-lock.json`

## Log
- 2026-10-01: done in commit `a3e2544` (merged to `main`, not pushed at the time).

## Related
- Decisions: [[D-005 Transparent floats]]
- Guides: [[UI and Colorscheme]]
