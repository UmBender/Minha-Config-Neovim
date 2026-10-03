-- Compile / run helpers for competitive programming (C++)
local M = {}

local common = { "-std=c++20", "-Wall", "-Wextra", "-Wshadow", "-DLOCAL" }
M.flags = vim.list_extend({ "-O2" }, common)
M.debug_flags = vim.list_extend({
  "-g",
  "-O0",
  "-D_GLIBCXX_DEBUG",
  "-D_GLIBCXX_DEBUG_PEDANTIC",
  "-fsanitize=address,undefined",
}, common)

M.template = vim.fn.stdpath("config") .. "/templates/cp.cpp"

-- Precompiled bits/stdc++.h: parsing it is most of a compile (~2 s -> ~0.75 s). One PCH per flag set in
-- <pch_root>/<key>/bits/stdc++.h.gch; `-I <key dir>` makes g++ use it in place of the header.
M.pch_root = vim.fn.stdpath("cache") .. "/pch"

local gcc ---@type {version: string, header: string, mtime: integer}?
local function gcc_info()
  if not gcc then
    local version = vim.system({ "g++", "--version" }, { text = true }):wait().stdout or ""
    local search = vim.system({ "g++", "-xc++", "-E", "-v", "-" }, { text = true, stdin = "" }):wait().stderr or ""
    local header, mtime = "", 0
    for dir in search:gmatch("\n (%S+)") do
      local stat = vim.uv.fs_stat(dir .. "/bits/stdc++.h")
      if stat then
        header, mtime = vim.fs.normalize(dir .. "/bits/stdc++.h"), stat.mtime.sec
        break
      end
    end
    gcc = { version = version, header = header, mtime = mtime }
  end
  return gcc
end

--- Cache dir of the PCH for `flags`; the key changes with the flags, the compiler and its headers.
---@param flags string[]
function M.pch_dir(flags)
  local g = gcc_info()
  local key = table.concat(vim.list_extend({ g.version, g.header, tostring(g.mtime) }, flags), "\0")
  return M.pch_root .. "/" .. vim.fn.sha256(key):sub(1, 16)
end

--- Compiler args to use the PCH of `flags` (a missing PCH is harmless, a stale one is ignored with a warning).
function M.pch_args(flags)
  return { "-I", M.pch_dir(flags), "-Winvalid-pch" }
end

--- Delete PCHs of other flag sets or compilers (each is 150+ MB).
function M.prune_pch()
  local keep = { [M.pch_dir(M.flags)] = true, [M.pch_dir(M.debug_flags)] = true }
  for name in vim.fs.dir(M.pch_root) do
    local dir = M.pch_root .. "/" .. name
    if not keep[dir] then
      vim.fn.delete(dir, "rf")
    end
  end
end

local building = {} ---@type table<string, fun(ok: boolean)[]>

--- Build the PCH for `flags` in the background (~7 s). `cb(ok)` runs when it is there.
---@param flags string[]
---@param cb? fun(ok: boolean)
---@return boolean started false when the PCH already exists (cb runs now) or is being built
function M.build_pch(flags, cb)
  local dir = M.pch_dir(flags)
  local gch = dir .. "/bits/stdc++.h.gch"
  if vim.uv.fs_stat(gch) then
    if cb then
      cb(true)
    end
    return false
  end
  if building[dir] then
    table.insert(building[dir], cb)
    return false
  end
  local header = gcc_info().header
  if header == "" then
    if cb then
      cb(false)
    end
    return false
  end
  building[dir] = { cb }
  vim.fn.mkdir(dir .. "/bits", "p")
  local tmp = ("%s.tmp%d"):format(gch, vim.uv.os_getpid())
  local cmd = vim.list_extend({ "g++" }, flags)
  vim.list_extend(cmd, { "-x", "c++-header", header, "-o", tmp })
  vim.system(cmd, { text = true }, function(res)
    local ok = res.code == 0 and vim.uv.fs_rename(tmp, gch) ~= nil
    if not ok then
      vim.uv.fs_unlink(tmp)
    end
    vim.schedule(function()
      local cbs = building[dir]
      building[dir] = nil
      if ok then
        pcall(M.prune_pch)
      else
        vim.notify(
          "Could not precompile bits/stdc++.h:\n" .. (res.stderr or ""),
          vim.log.levels.WARN,
          { title = "g++" }
        )
      end
      for _, f in pairs(cbs) do
        f(ok)
      end
    end)
  end)
  return true
end

--- g++ command line for `file` (release or debug flags, with their PCH).
---@param debug? boolean
function M.compile_cmd(file, exe, debug)
  local flags = debug and M.debug_flags or M.flags
  local cmd = vim.list_extend({ "g++" }, flags)
  vim.list_extend(cmd, M.pch_args(flags))
  return vim.list_extend(cmd, { file, "-o", exe })
end

local efm = table.concat({
  "%f:%l:%c: %trror: %m",
  "%f:%l:%c: %tarning: %m",
  "%f:%l:%c: %tote: %m",
  "%-G%.%#",
}, ",")

--- Fill an empty buffer with the template and put the cursor inside solve()
function M.insert_template(buf)
  buf = buf or 0
  if vim.fn.filereadable(M.template) == 0 then
    return
  end
  vim.api.nvim_buf_set_lines(buf, 0, -1, false, vim.fn.readfile(M.template))
  vim.api.nvim_buf_call(buf, function()
    if vim.fn.search("^void solve", "w") > 0 then
      vim.cmd("normal! j")
    end
  end)
end

-- Python templates (templates/py/<kind>.py): stdin solution / brute force, random generator, stress test, Project Euler
M.py_kinds = { "sol", "gen", "stress", "euler" }

function M.py_template_file(kind)
  return vim.fn.stdpath("config") .. "/templates/py/" .. kind .. ".py"
end

--- Template kind for a new Python file, from its name; nil for other scripts (they stay empty).
---@return string?
function M.py_template_for(path)
  local name = vim.fn.fnamemodify(path, ":t"):lower()
  if not name:match("%.py$") then
    return nil
  end
  if name:match("^stress") then
    return "stress"
  elseif name:match("^gen") then
    return "gen"
  elseif name:match("brute") or name:match("^sol") then
    return "sol"
  elseif name:match("^euler") or name:match("^pe?_?%d+%.py$") then
    return "euler"
  end
end

--- Fill `buf` with a Python template; `{{N}}` becomes the problem number in the file name (p42.py -> 42).
function M.insert_py_template(buf, kind)
  buf = buf == 0 and vim.api.nvim_get_current_buf() or buf
  local name = vim.fn.fnamemodify(vim.api.nvim_buf_get_name(buf), ":t")
  local n = name:match("(%d+)") or "?"
  local lines = vim.tbl_map(function(l)
    return (l:gsub("{{N}}", n))
  end, vim.fn.readfile(M.py_template_file(kind)))
  vim.api.nvim_buf_set_lines(buf, 0, -1, false, lines)
  for _, pattern in ipairs({ "^def solve", "^def gen", "^COMPILE" }) do
    for i, line in ipairs(lines) do
      if line:match(pattern) then
        local win = vim.fn.bufwinid(buf)
        if win ~= -1 then
          vim.api.nvim_win_set_cursor(win, { math.min(i + (pattern:match("def") and 1 or 0), #lines), 0 })
        end
        return
      end
    end
  end
end

--- Choose a Python template for the current buffer (preselecting the one its name suggests).
function M.pick_py_template()
  local buf = vim.api.nvim_get_current_buf()
  local guess = M.py_template_for(vim.api.nvim_buf_get_name(buf))
  local kinds = vim.list_slice(M.py_kinds)
  if guess then
    table.sort(kinds, function(a, b)
      return (a == guess) and b ~= guess
    end)
  end
  local labels = {
    sol = "solution / brute force (stdin)",
    gen = "random test generator (seed = argv[1])",
    stress = "stress test (gen vs sol vs brute)",
    euler = "Project Euler",
  }
  vim.ui.select(kinds, {
    prompt = "Python template",
    format_item = function(k)
      return ("%-7s %s"):format(k, labels[k])
    end,
  }, function(kind)
    if kind then
      M.insert_py_template(buf, kind)
    end
  end)
end

--- Run the current Python file in a floating terminal (`args` appended, e.g. a seed or a test count).
---@param args? string[]
function M.run_py(args)
  vim.cmd("silent! update")
  local file = vim.api.nvim_buf_get_name(0)
  Snacks.terminal.open(vim.list_extend({ "python3", file }, args or {}), {
    cwd = vim.fn.fnamemodify(file, ":h"),
    auto_close = false,
    win = {
      position = "float",
      title = (" %s "):format(vim.fn.fnamemodify(file, ":t")),
      title_pos = "center",
    },
  })
end

--- Compile the current file with g++; errors/warnings go to the quickfix list
---@param opts? {debug?: boolean, on_success?: fun(exe: string)}
function M.compile(opts)
  opts = opts or {}
  vim.cmd("silent! update")
  local file = vim.api.nvim_buf_get_name(0)
  local exe = vim.fn.fnamemodify(file, ":r")
  local cmd = M.compile_cmd(file, exe, opts.debug)
  -- the first compile without a PCH builds it for the next ones
  M.build_pch(opts.debug and M.debug_flags or M.flags)

  local start = vim.uv.hrtime()
  vim.system(
    cmd,
    { text = true },
    vim.schedule_wrap(function(res)
      local lines = vim.split(res.stderr or "", "\n", { trimempty = true })
      vim.fn.setqflist({}, " ", { title = "g++", lines = lines, efm = efm })
      if res.code ~= 0 then
        vim.notify("Compilation failed", vim.log.levels.ERROR, { title = "g++" })
        vim.cmd("copen")
        return
      end
      local ms = math.floor((vim.uv.hrtime() - start) / 1e6)
      local warnings = #vim.fn.getqflist()
      vim.notify(
        ("Compiled in %d ms%s"):format(ms, warnings > 0 and (" (%d warnings, see quickfix)"):format(warnings) or ""),
        warnings > 0 and vim.log.levels.WARN or vim.log.levels.INFO,
        { title = "g++" }
      )
      if opts.on_success then
        opts.on_success(exe)
      end
    end)
  )
end

--- Compile, then run the binary interactively in a floating terminal
---@param opts? {debug?: boolean}
function M.run(opts)
  opts = opts or {}
  M.compile({
    debug = opts.debug,
    on_success = function(exe)
      Snacks.terminal.open({ exe }, {
        cwd = vim.fn.fnamemodify(exe, ":h"),
        auto_close = false,
        win = {
          position = "float",
          title = (" %s%s "):format(vim.fn.fnamemodify(exe, ":t"), opts.debug and " [debug]" or ""),
          title_pos = "center",
        },
      })
    end,
  })
end

return M
