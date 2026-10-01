-- Tests for lua/util/fold.lua. Run: nvim --headless --clean -l tests/nvim/fold_test.lua
local root = vim.fn.fnamemodify(debug.getinfo(1, "S").source:sub(2), ":p:h:h:h")
package.path = root .. "/lua/?.lua;" .. root .. "/lua/?/init.lua;" .. package.path

local fold = require("util.fold")

local failures = 0
local function test(name, fn)
  local ok, err = pcall(fn)
  if not ok then
    failures = failures + 1
    io.stderr:write("FAIL " .. name .. ": " .. tostring(err) .. "\n")
  end
end
local function eq(got, want)
  if not vim.deep_equal(got, want) then
    error(("\n  got:  %s\n  want: %s"):format(vim.inspect(got), vim.inspect(want)), 2)
  end
end

local file = {
  "#include <bits/stdc++.h>", --  1
  "// Title: Base", --            2
  "// Usage: base();", --         3
  "int base() {", --              4
  "    return 1;", --             5
  "}", --                         6
  "// End: Base", --              7
  "", --                          8
  "// Title: No end", --          9
  "int x;", --                   10
  "// Title: Top", --            11
  "int top() { return 2; }", --  12
  "// End: Top", --              13
  "", --                         14
  "void solve() {", --           15
  "}", --                        16
}

--- A buffer with `file` shown in the current window, folded like a cpp buffer, all folds open.
local function setup(base)
  fold.base = base or function()
    return "0"
  end
  local buf = vim.api.nvim_create_buf(false, true)
  vim.api.nvim_buf_set_lines(buf, 0, -1, false, file)
  vim.api.nvim_set_current_buf(buf)
  vim.wo.foldlevel = 99 -- as in LazyVim
  fold.attach(buf)
  vim.cmd("%foldopen!")
  return buf
end

local function closed(lnum)
  return { vim.fn.foldclosed(lnum), vim.fn.foldclosedend(lnum) }
end

test("ranges pair each Title with its End", function()
  local buf = setup()
  eq(fold.ranges(buf), {
    { start = 2, stop = 7, title = "Base" },
    { start = 11, stop = 13, title = "Top" },
  })
end)

test("ranges follow edits", function()
  local buf = setup()
  vim.api.nvim_buf_set_lines(buf, 0, 0, false, { "// a new first line" })
  eq(fold.ranges(buf)[1], { start = 3, stop = 8, title = "Base" })
  vim.api.nvim_buf_set_lines(buf, 11, 12, false, {}) -- drop "// Title: Top"
  eq(#fold.ranges(buf), 1)
end)

test("templates are level-1 folds over the base foldexpr, shifted inside", function()
  local base = {}
  for i = 1, #file do
    base[i] = "0"
  end
  base[4], base[5], base[6] = ">1", "1", "<1" -- body of base()
  base[15], base[16] = ">1", "<1" -- body of solve()
  setup(function()
    return base[vim.v.lnum]
  end)
  local levels = {}
  for i = 1, #file do
    levels[i] = vim.fn.foldlevel(i)
  end
  eq(levels, { 0, 1, 1, 2, 2, 2, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1 })
end)

test("shift adds one level to foldexpr results", function()
  eq(fold.shift("0"), "1")
  eq(fold.shift("2"), "3")
  eq(fold.shift(">1"), ">2")
  eq(fold.shift("<3"), "<4")
  eq(fold.shift("="), "=")
  eq(fold.shift("a1"), "a1")
  eq(fold.shift(0), "1")
end)

test("close collapses every template, only templates", function()
  setup()
  fold.close(0)
  eq(closed(4), { 2, 7 })
  eq(closed(12), { 11, 13 })
  eq(closed(10), { -1, -1 })
  eq(closed(15), { -1, -1 })
end)

test("close by title leaves the other templates alone", function()
  setup()
  fold.close(0, { "Top" })
  eq(closed(4), { -1, -1 })
  eq(closed(12), { 11, 13 })
end)

test("close keeps the inner folds of a template as they were", function()
  setup(function()
    return ({ [4] = ">1", [5] = "1", [6] = "<1" })[vim.v.lnum] or "0"
  end)
  fold.close(0)
  vim.cmd("2foldopen")
  eq(closed(5), { -1, -1 })
end)

test("toggle closes all when any template is open, else opens all", function()
  setup()
  fold.close(0, { "Base" })
  fold.toggle(0)
  eq(closed(4), { 2, 7 })
  eq(closed(12), { 11, 13 })
  fold.toggle(0)
  eq(closed(4), { -1, -1 })
  eq(closed(12), { -1, -1 })
end)

test("attach uses the template foldexpr in the window and collapses the templates", function()
  setup()
  fold.attach(0)
  eq(closed(4), { 2, 7 })
  eq(vim.wo.foldmethod, "expr")
  eq(vim.wo.foldexpr, "v:lua.require'util.fold'.foldexpr()")
end)

if failures > 0 then
  os.exit(1)
end
print("fold_test: all passed")
