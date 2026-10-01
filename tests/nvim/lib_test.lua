-- Tests for lua/util/lib.lua. Run: nvim --headless --clean -l tests/nvim/lib_test.lua
local root = vim.fn.fnamemodify(debug.getinfo(1, "S").source:sub(2), ":p:h:h:h")
package.path = root .. "/lua/?.lua;" .. root .. "/lua/?/init.lua;" .. package.path

local lib = require("util.lib")
lib.root = root .. "/tests/nvim/fixtures/lib"

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

test("list parses headers", function()
  local items = lib.list()
  eq(vim.tbl_map(function(i) return i.id end, items), { "x/base", "x/top" })
  local top = items[2]
  eq(top.title, "Top")
  eq(top.description, "Fixture that requires x/base.")
  eq(top.requires, { "x/base" })
  eq(top.verify, "https://example.com/top")
  eq(top.file, lib.root .. "/x/top.cpp")
end)

test("resolve puts dependencies first", function()
  eq(lib.resolve("x/top"), { "x/base", "x/top" })
  eq(lib.resolve("x/base"), { "x/base" })
end)

test("insert goes above solve() with dependencies", function()
  local buf = buf_with({ "#include <bits/stdc++.h>", "using namespace std;", "", "void solve() {", "}", "", "int main() {}" })
  eq(lib.insert("x/top", buf), { "x/base", "x/top" })
  local l = lines(buf)
  eq(l[4], "// Title: Base")
  local top = vim.fn.index(l, "// Title: Top") + 1
  assert(top > 4, "Top inserted after Base")
  eq(l[#l - 3], "void solve() {")
  eq(l[#l - 4], "")
end)

test("insert skips templates already present", function()
  local buf = buf_with({ "// Title: Base", "int base() { return 1; }", "", "int main() {}" })
  eq(lib.insert("x/top", buf), { "x/top" })
  local l = lines(buf)
  eq(#vim.tbl_filter(function(s) return s == "// Title: Base" end, l), 1)
  eq(l[#l], "int main() {}")
  eq(lib.insert("x/top", buf), {})
end)

test("insert falls back to main() then to the cursor", function()
  local buf = buf_with({ "int main() {}" })
  lib.insert("x/base", buf)
  eq(lines(buf)[1], "// Title: Base")
  eq(lines(buf)[#lines(buf)], "int main() {}")

  local buf2 = buf_with({ "// nothing here", "" })
  vim.api.nvim_set_current_buf(buf2)
  vim.api.nvim_win_set_cursor(0, { 1, 0 })
  lib.insert("x/base", buf2)
  eq(lines(buf2)[1], "// nothing here")
  eq(lines(buf2)[2], "// Title: Base")
end)

test("unknown template errors", function()
  assert(not pcall(lib.insert, "x/nope", buf_with({})), "should error")
end)

if failures > 0 then
  os.exit(1)
end
print("lib_test: all passed")
