-- Tests for the performance tweaks (T-014). Run: nvim --headless --clean -l tests/nvim/perf_test.lua
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

--- First spec in lua/plugins/<file>.lua for `name` that sets `opts`.
local function spec(file, name)
  for _, s in ipairs(dofile(root .. "/lua/plugins/" .. file .. ".lua")) do
    if s[1] == name and s.opts then
      return s
    end
  end
  error("no spec with opts for " .. name .. " in " .. file)
end

test("have_query finds queries without compiling them", function()
  local perf = require("util.perf")
  local get, parse, calls = vim.treesitter.query.get, vim.treesitter.query.parse, 0
  vim.treesitter.query.get = function(...) calls = calls + 1 return get(...) end
  vim.treesitter.query.parse = function(...) calls = calls + 1 return parse(...) end
  local ok, err = pcall(function()
    eq(perf.have_query("c", "highlights"), true) -- shipped with Neovim
    eq(perf.have_query("c", "no-such-query"), false)
    eq(perf.have_query("no-such-lang", "highlights"), false)
  end)
  vim.treesitter.query.get, vim.treesitter.query.parse = get, parse
  assert(ok, err)
  eq(calls, 0)
end)

test("setup replaces LazyVim's have_query and keeps its cache", function()
  local perf = require("util.perf")
  _G.LazyVim = { treesitter = { _queries = {}, have_query = function() error("compiled") end } }
  perf.setup()
  eq(LazyVim.treesitter.have_query("c", "highlights"), true)
  eq(LazyVim.treesitter._queries["c:highlights"], true)
  _G.LazyVim = nil
end)

test("smear cursor is snappy", function()
  local o = spec("ui", "sphamba/smear-cursor.nvim").opts
  assert(o.stiffness >= 0.8 and o.trailing_stiffness >= 0.6, "normal mode dynamics")
  assert(o.stiffness_insert_mode >= 0.7 and o.trailing_stiffness_insert_mode >= 0.7, "insert mode dynamics")
  assert(o.distance_stop_animating >= 0.5, "stops early")
  assert(o.time_interval <= 10, "framerate")
end)

test("smooth scroll and the indent scope animation are off", function()
  local o = spec("ui", "folke/snacks.nvim").opts
  eq(o.scroll, { enabled = false })
  eq(o.indent.animate, { enabled = false })
  assert(#o.indent.indent.hl == 7, "rainbow indent kept")
end)

test("no trouble symbols in lualine (dropbar shows them)", function()
  vim.g.trouble_lualine = nil
  dofile(root .. "/lua/config/options.lua")
  eq(vim.g.trouble_lualine, false)
end)

if failures > 0 then
  os.exit(1)
end
print("ok")
