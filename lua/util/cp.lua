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

--- Compile the current file with g++; errors/warnings go to the quickfix list
---@param opts? {debug?: boolean, on_success?: fun(exe: string)}
function M.compile(opts)
  opts = opts or {}
  vim.cmd("silent! update")
  local file = vim.api.nvim_buf_get_name(0)
  local exe = vim.fn.fnamemodify(file, ":r")
  local cmd = vim.list_extend({ "g++" }, opts.debug and M.debug_flags or M.flags)
  vim.list_extend(cmd, { file, "-o", exe })

  local start = vim.uv.hrtime()
  vim.system(cmd, { text = true }, vim.schedule_wrap(function(res)
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
  end))
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
