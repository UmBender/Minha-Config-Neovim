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
lib/<area>/<name>.cpp    C++ template library (inserted with <leader>rl), see "Template library"
examples/<area>/<name>.cpp   runnable usage example per template (checked by the runner)
tests/                   tests for lib/ (tests/<area>/<name>.cpp) and nvim helpers (tests/nvim/)
tasks/Library/           generated docs, one page per template (tests/run.py --write-docs)
lua/util/lib.lua         library picker / insertion
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

## Task pipeline (mandatory for every task)

1. **Read the task**: create or update `tasks/Tasks/T-NNN <title>.md` (status `doing`), restate the
   goal and requirements, break it into a checklist. Ask the user only for decisions that are theirs.
2. **Branch**: `git checkout -b task/T-NNN-<slug>` from an up-to-date `main`.
3. **Tests first**: write or extend the tests for the change (`tests/...`). Run them and see the new
   ones fail for the right reason.
4. **Implement** until `python3 tests/run.py` passes **in full** (not only the new tests). A change
   that breaks an existing test either gets fixed, or the test is changed deliberately and the reason
   is logged in the task note.
5. **Document**: update the task log, guides (keymaps, usage) and decisions in the vault.
6. **Commit** on the branch (conventional message), set the task `status: done`.
7. **Merge**: `git checkout main && git merge --ff-only task/T-NNN-<slug> && git branch -d task/T-NNN-<slug>`.
   Do **not** push unless the user asks.
8. **Clear context**: once a task is merged, stop and don't start the next task in the same session.
   Ask the user to run `/clear` (an agent can't clear its own context), then begin the next task
   fresh from `AGENTS.md`, `tasks/Home.md` and the task note. So the task note has to hold
   everything needed to resume a task: its plan, log and open decisions.

The user has authorized steps 6 and 7 as part of this pipeline.

## Template library (`lib/`)

Based on the notebook, but **not 1:1**: the notebook minimizes typing; the library favors
flexibility, since typing is free here (the picker inserts the code).

- Self-contained: compiles with only `#include <bits/stdc++.h>` + `using namespace std;` above it.
  No `#include`, `#define`, `#pragma` or `using namespace std` inside templates (lint enforces it).
- Generic types (`template <class T>`), operations as lambdas/functors (CTAD-friendly constructors),
  richer APIs than the notebook (e.g. `maxRight`/`minLeft`, sizes, path recovery).
- Conventions: 0-indexed, half-open ranges `[l, r)`, 4-space indent, warning-free under
  `-Wall -Wextra -Wshadow`.
- Header (parsed by the picker and the runner):
  ```cpp
  // Title: Fenwick tree
  // Description: One line shown in the picker.
  // Usage:
  //   Fenwick<long long> fw(n); fw.add(i, x); fw.sum(l, r);
  // Complexity: O(log n) per operation.
  // Verify: https://judge.yosupo.jp/problem/point_add_range_sum   (optional, repeatable)
  // Requires: dsa/other-template                                 (optional)
  ```
- **Presets**: every generic template also offers its most common uses as one-liners, inside the
  same file (so they're inserted with it): default template arguments (`Fenwick fw(n)` is
  `long long`), factory functions (`auto seg = minSegtree(a)`) or small wrapper structs with plain
  values in and out (`RangeAddSum<long long> seg(a); seg.add(l, r, x); seg.sum(l, r)`). List them in
  an optional `// Presets:` header key. Presets are tested like everything else.
- **Example**: every template has `examples/<area>/<name>.cpp`, a complete small solution that
  `#include "<area>/<name>.cpp"` and shows the typical use (presets first, then the general form
  when it adds something). Header keys: `Problem:` (statement), `Input:` (stdin, optional),
  `Output:` (exact expected stdout). The runner compiles it like a test, feeds `Input` and compares
  `Output`, so examples can't rot.
- **Docs**: `tasks/Library/<area>/<Title>.md` (one page per template) and `tasks/Library/Library.md`
  (catalog) are **generated** from the template header + example by
  `python3 tests/run.py --write-docs`. Never edit them by hand; a normal run fails when they are
  stale. Titles name the pages: no `\ / : # ^ [ ] |`.
- `// Pending:` (temporary, migration only) marks a template whose example/presets are not done yet;
  lint skips the example requirement for it. Remove it when the template is migrated.
- Every template has a test at `tests/<area>/<name>.cpp` (lint fails otherwise): fixed edge cases
  plus a randomized **stress test against a brute force**. Tests include `test.h` (`tests/include/`, `CHECK`,
  `CHECK_EQ`, `CHECK_NEAR`, `test::rnd`, ...) and the template via `#include "<area>/<name>.cpp"`.
- `python3 tests/run.py [filter]` runs lint, standalone compilation of each template, the C++
  tests (`-Werror`, ASan/UBSan, `_GLIBCXX_DEBUG`), the examples, the docs check and the nvim
  tests. Builds are cached in `tests/.build/`. `--write-docs` regenerates `tasks/Library/`.
- Parallel work (several agents): each agent works in its own git worktree/branch, touches only
  its own templates/tests/examples/generated pages and its own task note. Don't change template
  `Title`/`Description` of others (they feed the shared catalog page); the catalog
  (`tasks/Library/Library.md`) is regenerated by whoever merges.

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

- Pipeline tasks commit and merge on their own (see *Task pipeline*). Outside the pipeline,
  commit only when asked. Push **only when asked**.
- Don't commit directly on `main`: commit on a short-lived branch, then fast-forward merge
  into `main` when the user asks ("commit and merge to main"). Push only when asked.
- Stage files explicitly or check `git status` first: `.claude/` (agent worktrees) is ignored, but
  never rely on `git add -A` blindly.
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

- GCC 16's `bits/stdc++.h` no longer includes `<cassert>`: `assert` needs `#include <cassert>`.
  Templates must not use it (lint rejects it).
- Test includes resolve `-I lib` then `-I tests/include`; never put tests on the include path
  (a missing template would make a test include itself).
- After writing tests for a template, mutate the template (reintroduce a plausible bug) and make
  sure the suite fails. An infinite loop shows up as a 60 s timeout.

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
