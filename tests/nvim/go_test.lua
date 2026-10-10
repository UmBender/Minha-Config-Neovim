-- Tests for the Go setup (T-027). Run: nvim --headless --clean -l tests/nvim/go_test.lua
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

test("lazy.lua imports the Go and test extras", function()
  local lazy = table.concat(vim.fn.readfile(root .. "/lua/config/lazy.lua"), "\n")
  for _, extra in ipairs({ "lang.go", "test.core" }) do
    assert(lazy:find('import = "lazyvim.plugins.extras.' .. extra .. '"', 1, true), extra .. " not imported")
  end
end)

test("go_run_cmd runs the file's package from its directory", function()
  local cp = require("util.cp")
  eq(cp.go_run_cmd("/p/cmd/app/main.go"), { cmd = { "go", "run", "." }, cwd = "/p/cmd/app" })
  eq(cp.go_run_cmd("/p/main.go", { "-n", "3" }), { cmd = { "go", "run", ".", "-n", "3" }, cwd = "/p" })
end)

test("go ftplugin maps run keys buffer-locally", function()
  vim.g.mapleader = " "
  local buf = vim.api.nvim_create_buf(true, false)
  vim.api.nvim_set_current_buf(buf)
  vim.cmd.source(root .. "/after/ftplugin/go.lua")
  local descs = {}
  for _, m in ipairs(vim.api.nvim_buf_get_keymap(buf, "n")) do
    descs[m.lhs:gsub("^ ", "<leader>")] = m.desc
  end
  eq(descs["<leader>rr"], "Run Package")
  eq(descs["<leader>ri"], "Run Package with Arguments")
  vim.api.nvim_set_current_buf(vim.api.nvim_create_buf(true, false))
  eq(vim.fn.maparg("<leader>rr", "n"), "") -- not global
end)

if failures > 0 then
  os.exit(1)
end
print("ok")
