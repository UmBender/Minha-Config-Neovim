-- Collapsible library templates: each `// Title: X` .. `// End: X` block (written by util.lib) is a level-1 fold
-- on top of the base foldexpr (LazyVim's treesitter folds), whose levels shift by one inside a template.
local M = {}

M.expr = "v:lua.require'util.fold'.foldexpr()"

--- Fold level of the current line (`v:lnum`) without templates.
---@return string|integer
function M.base()
  return _G.LazyVim and LazyVim.treesitter.foldexpr() or "0"
end

---@class cp.FoldRange
---@field start integer 1-based `// Title:` line
---@field stop integer  1-based `// End:` line
---@field title string

local cache = {} ---@type table<integer, {tick: integer, ranges: cp.FoldRange[], at: table<integer, cp.FoldRange>}>

local function scan(buf)
  buf = buf == 0 and vim.api.nvim_get_current_buf() or buf
  local tick = vim.api.nvim_buf_get_changedtick(buf)
  local c = cache[buf]
  if c and c.tick == tick then
    return c
  end
  c = { tick = tick, ranges = {}, at = {} }
  local open ---@type cp.FoldRange?
  for i, line in ipairs(vim.api.nvim_buf_get_lines(buf, 0, -1, false)) do
    local title = line:match("^// Title: (.*)$")
    if title then
      open = { start = i, title = title }
    elseif open and line == "// End: " .. open.title then
      open.stop = i
      c.ranges[#c.ranges + 1] = open
      for l = open.start, i do
        c.at[l] = open
      end
      open = nil
    end
  end
  cache[buf] = c
  return c
end

--- Template blocks in `buf`, in order. A Title without its End line is not a template block.
---@return cp.FoldRange[]
function M.ranges(buf)
  return scan(buf).ranges
end

--- One more level for a foldexpr result (">1" -> ">2", "1" -> "2"); relative ones ("=", "a1", ...) stay.
---@param level string|integer
---@return string
function M.shift(level)
  local prefix, n = tostring(level):match("^([<>]?)(%d+)$")
  return n and prefix .. (tonumber(n) + 1) or tostring(level)
end

function M.foldexpr()
  local lnum = vim.v.lnum
  local r = scan(vim.api.nvim_get_current_buf()).at[lnum]
  if not r then
    return M.base()
  elseif lnum == r.start then
    return ">1"
  elseif lnum == r.stop then
    return "<1"
  end
  return M.shift(M.base())
end

--- Call `fn(range)` for the templates of `buf` (named in `titles`, if given) in every window folding them.
local function each_window(buf, titles, fn)
  buf = buf == 0 and vim.api.nvim_get_current_buf() or buf
  local want = titles and {} or nil
  for _, t in ipairs(titles or {}) do
    want[t] = true
  end
  for _, win in ipairs(vim.fn.win_findbuf(buf)) do
    if vim.wo[win].foldexpr == M.expr then
      vim.api.nvim_win_call(win, function()
        for _, r in ipairs(M.ranges(buf)) do
          if not want or want[r.title] then
            fn(r)
          end
        end
      end)
    end
  end
end

--- Collapse the templates of `buf` (only those named in `titles`, if given).
---@param titles? string[]
function M.close(buf, titles)
  each_window(buf, titles, function(r)
    if vim.fn.foldclosed(r.start) == -1 then
      vim.cmd(r.start .. "foldclose")
    end
  end)
end

--- Collapse every template if any is open in the current window, else open them all.
function M.toggle(buf)
  local any_open = false
  for _, r in ipairs(M.ranges(buf)) do
    any_open = any_open or vim.fn.foldclosed(r.start) == -1
  end
  if any_open then
    M.close(buf)
  else
    each_window(buf, nil, function(r)
      if vim.fn.foldclosed(r.start) ~= -1 then
        vim.cmd(r.start .. "foldopen")
      end
    end)
  end
end

--- Use template folds in the current window (showing `buf`) and collapse the templates.
function M.attach(buf)
  vim.opt_local.foldmethod = "expr"
  vim.opt_local.foldexpr = M.expr
  M.close(buf)
end

return M
