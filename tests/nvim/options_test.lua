-- Tests for lua/config/options.lua (T-028). Run: nvim --headless --clean -l tests/nvim/options_test.lua
local root = vim.fn.fnamemodify(debug.getinfo(1, "S").source:sub(2), ":p:h:h:h")
package.path = root .. "/lua/?.lua;" .. root .. "/lua/?/init.lua;" .. package.path

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

dofile(root .. "/lua/config/options.lua")

test("tabs are drawn blank (Go indents with tabs), other markers stay", function()
  local lcs = vim.opt.listchars:get()
  eq(lcs.tab, "  ")
  eq(lcs.trail, "-")
  eq(lcs.nbsp, "+")
end)

if failures > 0 then
  os.exit(1)
end
print("ok")
