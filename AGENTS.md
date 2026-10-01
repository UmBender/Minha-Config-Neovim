# AGENTS.md

Guide for AI agents (and humans) working on this Neovim config.

## What this is

Personal Neovim config (Neovim 0.12+) built on [LazyVim](https://www.lazyvim.org), focused on
**C++ competitive programming (ICPC)**. The look (Kanagawa Dragon, transparent, BENDER VIM
dashboard, bubbles lualine, rainbow indent, dropbar winbar) is intentional and must be preserved.

## Layout

```
init.lua                 bootstrap -> lua/config/lazy.lua
lua/config/              LazyVim conventions: lazy.lua, options.lua, keymaps.lua, autocmds.lua
lua/plugins/             one file per area: colorscheme.lua, ui.lua, cpp.lua
lua/util/cp.lua          g++ compile/run helpers used by the <leader>r keymaps
after/ftplugin/cpp.lua   buffer-local C++ keymaps (<leader>r group)
templates/cp.cpp         template for new *.cpp files and CompetiTest received problems
snippets/cpp.json        VS Code-style snippets, loaded by blink.cmp
.clang-format            fallback style when a project has none
lazy-lock.json           plugin lockfile (commit it)
tasks/                   Obsidian vault: tasks, decisions, guides (see below)
notebook -> ~/gempro-notebook   ICPC library symlink, NOT tracked (see below)
```

## The notebook (`notebook/`)

- Symlink to `~/gempro-notebook`, the team's ICPC library. It is **read-only reference material**
  used as the source for templates/snippets in this config.
- **Never `git add` it** (it is in `.gitignore`) and never edit files inside it.
- Structure: `content/<area>/*.cpp` (areas: `dsa`, `graph`, `math`, `strings`, `geometry`, `tree`,
  `contest`), with descriptions (in Portuguese) in each `content/<area>/main.tex`.
- Each `.cpp` file repeats the common header/macros, then the reusable part sits after
  `// begin template //` (optionally ending at `// end template //`). `scripts/prepare.py` shows how
  the notebook itself extracts it. Code uses **tabs** and `-std=c++20`.
- `content/contest/template.cpp` is the team's contest template; `content/contest/.bashrc` has the
  shell helpers used in contests (`c`, `cs`, `gen`, `rr`, `chk`).

## Conventions

- Follow LazyVim idioms: extend plugins with `opts` (table or `function(_, opts)`), don't fork
  LazyVim specs. Check the upstream spec in `~/.local/share/nvim/lazy/LazyVim/lua/lazyvim/` before
  overriding. Lists in `opts` are **replaced**, not merged.
- Lua style: 2 spaces, 120 columns (`stylua.toml`). Match the comment density of nearby code.
- Keep C++ keymaps buffer-local in `after/ftplugin/cpp.lua` under `<leader>r`.
- Compiler flags live in two places that must stay in sync: `lua/util/cp.lua` and the
  CompetiTest `compile_command` in `lua/plugins/cpp.lua`.
- Extras are imported in `lua/config/lazy.lua` (not via `lazyvim.json`).

## Git workflow (user preferences)

- Commit or push **only when asked**.
- Don't commit directly on `main`: commit on a short-lived branch, then fast-forward merge
  into `main` when the user asks ("commit and merge to main"). Push only when asked.
- Never commit files the user drops in temporarily for reference (e.g. a terminal palette
  `.toml`), nor the `notebook` symlink.
- Commit messages: conventional style (`feat:`, `fix:`, ...), short bullet body.

## Testing changes

`nvim --headless` never fires `UIEnter`, so LazyVim's `VeryLazy` plugins (format-on-save, the
dashboard, keymaps, ...) **don't load headless**. Run real sessions in a pty instead:

```python
# pty_run.py: python3 pty_run.py nvim file.cpp -c "luafile test.lua"
import os, pty, sys, time, select
pid, fd = pty.fork()
if pid == 0:
    os.environ["TERM"] = "xterm-256color"
    os.execvp(sys.argv[1], sys.argv[1:])
end = time.time() + 40
while time.time() < end:
    r, _, _ = select.select([fd], [], [], 0.5)
    if r:
        try: os.read(fd, 65536)
        except OSError: break
    if os.waitpid(pid, os.WNOHANG)[0]: break
```

In the test script use `vim.defer_fn`, write results to a file, then `qa!`. The
`E1568 ... DSR request` message is a pty artifact, ignore it.

- Install/update plugins: `nvim --headless "+Lazy! sync" +qa`
- Mason commands need the plugin loaded first when headless:
  `nvim --headless -c "lua require('lazy').load({plugins={'mason.nvim'}})" -c "MasonInstall <pkg>" -c qa`
  (quitting early aborts installs; wait on the package's `install():once('closed', ...)`).

## Known gotchas

- clangd 23 requires a value for `--function-arg-placeholders` (`=1`); LazyVim's default works only
  by accident. See `tasks/Decisions/D-003`.
- clangd ignores inline `--fallback-style={...}`; formatting goes through conform + clang-format
  with `.clang-format` as fallback. See `tasks/Decisions/D-002`.
- Kanagawa is transparent; floats are forced transparent too (`ui.float`) so the explorer
  matches the editor. See `tasks/Decisions/D-005`.

## Task tracking (`tasks/` Obsidian vault)

Every non-trivial change is tracked in the vault. **Keep it up to date as part of the work**,
not afterwards.

- `tasks/Home.md`: index. Add every new note there.
- `tasks/Tasks/T-NNN <title>.md`: one per task. Frontmatter `status`: `todo | doing | done | blocked`.
  Log progress, sub-steps and links to decisions.
- `tasks/Decisions/D-NNN <title>.md`: one per non-obvious decision (context, decision,
  consequences). Never rewrite history: supersede with a new decision and link both.
- `tasks/Guides/*.md`: user-facing docs (keymaps, workflows). **When a keymap, command or workflow
  changes, update the matching guide in the same change.**
- `tasks/Templates/`: Obsidian templates for the notes above. Use them for new notes.
- Use `[[wikilinks]]` between notes, ISO dates (`YYYY-MM-DD`) and the next free number.
