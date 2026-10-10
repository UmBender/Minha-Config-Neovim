---
updated: 2026-10-10
tags: [guide]
---
# Keymaps

`<leader>` is **Space**. Press it and wait to see every group in which-key; `<leader>sk` searches all
keymaps. LazyVim defaults are listed only where they're useful day to day.

## Competitive programming (C++ buffers only, `<leader>r`)

See [[Competitive Programming]] for the workflow.

| Key | Action |
| --- | ------ |
| `<leader>rr` | Compile (`-O2`) and run in a floating terminal |
| `<leader>rd` | Compile with sanitizers + `_GLIBCXX_DEBUG` and run |
| `<leader>rc` | Compile only; errors/warnings go to quickfix |
| `<leader>rn` | Insert the solution template into the current buffer |
| `<leader>rl` | Insert from the template library (picker; structures with variants open a second menu: normal, educational, common uses; see [[Template Library]]) |
| `<leader>rf` | Collapse all library templates (or open them all if all are collapsed); `za` toggles the one under the cursor. See [[Template Library]] |
| `<leader>rt` | Run all test cases |
| `<leader>rT` | Run test cases without recompiling |
| `<leader>ru` | Show the test cases UI |
| `<leader>ra` / `re` / `rx` | Add / edit / delete a test case |
| `<leader>rp` | Receive a problem from Competitive Companion |
| `<leader>rP` | Receive a whole contest |
| `<leader>rR` | Receive test cases only (for the current file) |

## Competitive programming (Python buffers, `<leader>r`)

See [[Python]] for the workflow.

| Key | Action |
| --- | ------ |
| `<leader>rr` | Run with `python3` in a floating terminal |
| `<leader>ri` | Run with arguments (e.g. a seed for `gen.py`, a test count for `stress.py`) |
| `<leader>rn` | Insert a Python template: sol / gen / stress / euler |
| `<leader>rl` | Insert from the Python library (`pylib/`) |
| `<leader>rf` | Collapse / open all library templates |
| `<leader>rt` / `ru` | Run test cases / show the test cases UI (CompetiTest, `python3`) |
| `<leader>ra` / `re` / `rx` | Add / edit / delete a test case |

## Go (Go buffers, `<leader>r` and `<leader>t`)

See [[LSP, Formatting and Debugging#Go]].

| Key | Action |
| --- | ------ |
| `<leader>rr` | `go run .` in the file's directory (its package) in a floating terminal |
| `<leader>ri` | Same, with program arguments |
| `<leader>tr` / `tt` / `tT` | Run the nearest test / the file's tests / every test (neotest) |
| `<leader>tl` / `tS` | Run the last test again / stop |
| `<leader>ts` / `to` / `tO` | Test summary tree / output of a test / output panel |
| `<leader>tw` | Watch the file: rerun its tests on save |
| `<leader>td` | Debug the nearest test (delve) |
| `<leader>co` | Organize imports |

## Files and search

| Key | Action |
| --- | ------ |
| `<leader><space>` / `<leader>ff` | Find files |
| `<leader>fr` | Recent files |
| `<leader>/` / `<leader>sg` | Grep the project |
| `<leader>sw` | Grep the word under the cursor / selection |
| `<leader>e` / `<leader>E` | Explorer (root dir / cwd) |
| `<leader>,` | Open buffers |
| `<leader>fc` | Find a config file |
| `<leader>sR` | Resume the last picker |

## Buffers, windows, terminal

| Key | Action |
| --- | ------ |
| `<S-h>` / `<S-l>` | Previous / next buffer |
| `<leader>bd` | Close buffer |
| `<C-s>` | Save (formats on save) |
| `<C-h/j/k/l>` | Move between windows |
| `<leader>-` / `<leader>\|` | Split below / right |
| `<C-/>` / `<leader>ft` | Toggle terminal |
| `<leader>qq` | Quit all |

## Code (LSP)

| Key | Action |
| --- | ------ |
| `gd` / `gr` / `gI` | Definition / references / implementation |
| `K` | Hover docs |
| `<leader>ca` | Code action |
| `<leader>cr` | Rename |
| `<leader>cf` | Format |
| `<leader>cd` | Line diagnostics |
| `]d` / `[d` | Next / previous diagnostic |
| `<leader>xx` | Diagnostics list (Trouble) |
| `<leader>xQ` | Quickfix list (Trouble), e.g. after `<leader>rc` |
| `<leader>cs` | Symbols outline (Trouble) |
| `<leader>;` | Pick a symbol from the winbar (dropbar) |
| `<leader>uf` / `<leader>uF` | Toggle format on save (global / buffer) |

## Debugging (`<leader>d`, codelldb for C++, delve for Go)

| Key | Action |
| --- | ------ |
| `<leader>db` | Toggle breakpoint |
| `<leader>dB` | Conditional breakpoint |
| `<leader>dc` | Run / continue |
| `<leader>dC` | Run to cursor |
| `<leader>dO` / `di` / `do` | Step over / into / out |
| `<leader>du` | Toggle DAP UI |
| `<leader>de` | Evaluate expression |
| `<leader>dt` | Terminate |

## Completion and snippets

| Key | Action |
| --- | ------ |
| `<CR>` | Accept completion |
| `<C-space>` | Open completion |
| `<C-n>` / `<C-p>` | Next / previous item |
| `<Tab>` / `<S-Tab>` | Jump between snippet fields |

## Dashboard

`f` find file, `n` new file, `g` find text, `r` recent, `c` config, `e` notes (`~/notes/NOTES.md`),
`s` restore session, `l` Lazy, `q` quit.

## UI toggles (`<leader>u`)

`<leader>uC` colorscheme picker, `<leader>uw` wrap, `<leader>ul` line numbers,
`<leader>ud` diagnostics, `<leader>uh` inlay hints.
