-- Tests for the Python side of lua/util/{lib,fold,cp}.lua. Run: nvim --headless --clean -l tests/nvim/py_test.lua
local root = vim.fn.fnamemodify(debug.getinfo(1, "S").source:sub(2), ":p:h:h:h")
package.path = root .. "/lua/?.lua;" .. root .. "/lua/?/init.lua;" .. package.path

local lib = require("util.lib").python
lib.root = root .. "/tests/nvim/fixtures/pylib"
local fold = require("util.fold")
local cp = require("util.cp")

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
local function buf_with(lines)
  local buf = vim.api.nvim_create_buf(false, true)
  vim.api.nvim_buf_set_lines(buf, 0, -1, false, lines)
  return buf
end
local function lines(buf)
  return vim.api.nvim_buf_get_lines(buf, 0, -1, false)
end

test("the C++ library is unchanged", function()
  local cpp = require("util.lib")
  eq(cpp.root, vim.fn.stdpath("config") .. "/lib")
  eq(cpp.ext, "cpp")
  eq(cpp.comment, "//")
  eq(lib.ext, "py")
  eq(lib.comment, "#")
end)

test("python list parses # headers", function()
  local items = lib.list()
  eq(
    vim.tbl_map(function(i)
      return i.id
    end, items),
    { "x/base", "x/top" }
  )
  eq(items[2].title, "Py top")
  eq(items[2].requires, { "x/base" })
  eq(items[2].usage, "\ntop()")
  eq(items[1].variants, {})
end)

test("python insert goes above def solve with # End lines", function()
  local buf = buf_with({ "import sys", "", "", "def solve():", "    pass" })
  eq(lib.insert("x/top", buf), { "x/base", "x/top" })
  local l = lines(buf)
  eq(l[4], "# Title: Py base")
  assert(vim.tbl_contains(l, "# End: Py base"), "Base closed")
  assert(vim.tbl_contains(l, "# End: Py top"), "Top closed")
  eq(l[#l - 1], "def solve():")
  eq(lib.insert("x/top", buf), {})
end)

test("python insert falls back to def gen, def main, if __name__", function()
  for _, anchor in ipairs({ "def gen():", "def main():", 'if __name__ == "__main__":' }) do
    local buf = buf_with({ "import random", anchor, "    pass" })
    lib.insert("x/base", buf)
    eq(lines(buf)[2], "# Title: Py base")
    eq(lines(buf)[#lines(buf) - 1], anchor)
  end
end)

test("python resolve and unknown ids", function()
  eq(lib.resolve("x/top"), { "x/base", "x/top" })
  assert(not pcall(lib.insert, "x/nope", buf_with({})), "should error")
end)

test("for_filetype picks the library", function()
  local L = require("util.lib")
  eq(L.for_filetype("cpp"), L)
  eq(L.for_filetype("python"), L.python)
  eq(L.for_filetype("lua"), nil)
end)

test("folds pair # Title with # End", function()
  fold.base = function()
    return "0"
  end
  local buf = buf_with({ "import sys", "# Title: A", "x = 1", "# End: A", "# Title: B", "# End: Bx", "// End: A" })
  eq(fold.ranges(buf), { { start = 2, stop = 4, title = "A" } })
  local mixed = buf_with({ "// Title: A", "# End: A", "// End: A" })
  eq(fold.ranges(mixed), { { start = 1, stop = 3, title = "A" } })
end)

test("python template chosen by file name", function()
  local cases = {
    ["gen.py"] = "gen",
    ["/x/y/gen_trees.py"] = "gen",
    ["stress.py"] = "stress",
    ["brute.py"] = "sol",
    ["a_brute.py"] = "sol",
    ["sol.py"] = "sol",
    ["p12.py"] = "euler",
    ["pe700.py"] = "euler",
    ["euler.py"] = "euler",
    ["Euler_5.py"] = "euler",
    ["script.py"] = false,
    ["setup.py"] = false,
    ["p.py"] = false,
    ["gen.txt"] = false,
  }
  for path, want in pairs(cases) do
    eq({ path, cp.py_template_for(path) or false }, { path, want })
  end
end)

test("every python template kind has a file", function()
  for _, kind in ipairs(cp.py_kinds) do
    eq(vim.fn.filereadable(cp.py_template_file(kind)), 1)
  end
end)

test("insert_py_template fills the buffer and puts the cursor in the body", function()
  local buf = vim.api.nvim_create_buf(true, false)
  vim.api.nvim_buf_set_name(buf, "/tmp/p42.py")
  vim.api.nvim_set_current_buf(buf)
  cp.insert_py_template(buf, "euler")
  local l = lines(buf)
  eq(l[1], "# Project Euler 42")
  eq(l[2], "# https://projecteuler.net/problem=42")
  eq(vim.api.nvim_buf_get_lines(buf, vim.fn.line(".") - 2, vim.fn.line("."), false)[1], "def solve(n):")

  local sol = vim.api.nvim_create_buf(true, false)
  vim.api.nvim_buf_set_name(sol, "/tmp/brute.py")
  vim.api.nvim_set_current_buf(sol)
  cp.insert_py_template(sol, "sol")
  eq(lines(sol)[vim.fn.line(".") - 1], "def solve():")

  local euler = vim.api.nvim_create_buf(true, false)
  vim.api.nvim_buf_set_name(euler, "/tmp/euler.py")
  cp.insert_py_template(euler, "euler")
  eq(lines(euler)[1], "# Project Euler ?")
end)

if failures > 0 then
  os.exit(1)
end
print("py_test: all passed")
