# Bender's Neovim

[LazyVim](https://www.lazyvim.org)-based config, focused on C++ competitive programming.

## Layout

```
init.lua                 bootstrap
lua/config/              options, keymaps, autocmds, lazy.nvim setup (LazyVim conventions)
lua/plugins/
  colorscheme.lua        kanagawa dragon, higher contrast palette, rainbow indent colors
  ui.lua                 BENDER VIM dashboard, bubbles lualine, rainbow indent, dropbar winbar
  cpp.lua                clangd, clang-format, CompetiTest
lua/util/cp.lua          g++ compile / run helpers
after/ftplugin/cpp.lua   C++ keymaps (<leader>r)
templates/cp.cpp         template for new .cpp files and received problems
snippets/cpp.json        extra snippets (fori, all, vread, yesno)
.clang-format            fallback style when a project has none
AGENTS.md                conventions for agents working on this config
tasks/                   Obsidian vault: tasks, decisions and guides (keymaps, workflows)
```

Enabled LazyVim extras: `lang.clangd`, `dap.core`, `util.mini-hipatterns`. Use `:LazyExtras` for more languages.

## Competitive programming

New `*.cpp` files start from `templates/cp.cpp`. Builds define `LOCAL`, so `dbg(...)` prints to stderr
locally and compiles to nothing on the judge. The binary goes next to the source file.

| Key          | Action                                                           |
| ------------ | ---------------------------------------------------------------- |
| `<leader>rr` | Compile (`-O2`) and run in a floating terminal                   |
| `<leader>rd` | Compile with sanitizers + `_GLIBCXX_DEBUG` and run               |
| `<leader>rc` | Compile only; errors/warnings go to the quickfix list            |
| `<leader>rn` | Insert template into the current buffer                          |
| `<leader>rt` | Run all test cases (`<leader>rT` without recompiling)            |
| `<leader>ru` | Show test cases UI                                               |
| `<leader>ra` | Add test case (`re` edit, `rx` delete)                           |
| `<leader>rp` | Receive problem from [Competitive Companion]; `rP` contest, `rR` test cases |

Debugging (codelldb) uses the LazyVim `dap.core` keymaps under `<leader>d`.

[Competitive Companion]: https://github.com/jmerle/competitive-companion
