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

test("no smear cursor (T-022: its animation kept the screen moving ~90 ms after every jump)", function()
  local lazy = table.concat(vim.fn.readfile(root .. "/lua/config/lazy.lua"), "\n")
  assert(not lazy:find("smear", 1, true), "smear-cursor extra still imported")
  for _, s in ipairs(dofile(root .. "/lua/plugins/ui.lua")) do
    assert(s[1] ~= "sphamba/smear-cursor.nvim", "smear spec still in ui.lua")
  end
end)

--- Scratch buffer with filetype `ft` (FileType autocmds fire).
local function buffer(ft, lines)
  local buf = vim.api.nvim_create_buf(true, false)
  vim.api.nvim_buf_set_lines(buf, 0, -1, false, lines or { "int main() {", "  return 0;", "}" })
  vim.bo[buf].filetype = ft
  return buf
end

--- Let scheduled callbacks run.
local function tick()
  vim.wait(50, function() return false end)
end

test("when_ready defers the first buffer of a language by one tick, later ones run at once", function()
  local perf = require("util.perf")
  local a, b = buffer("c"), buffer("c")
  local ran = {}
  eq(perf.when_ready(a, function() ran[#ran + 1] = "a1" end), false)
  eq(perf.when_ready(a, function() ran[#ran + 1] = "a2" end), false)
  eq(perf.when_ready(b, function() ran[#ran + 1] = "b" end), false)
  eq(ran, {})
  tick()
  eq(ran, { "a1", "a2", "b" })
  eq(perf.when_ready(buffer("c"), function() error("must not be queued") end), true)
end)

test("when_ready drops queued work for deleted buffers and buffers that changed language", function()
  local perf = require("util.perf")
  local gone, changed, ran = buffer("lua"), buffer("lua"), {}
  perf.when_ready(gone, function() ran[#ran + 1] = "gone" end)
  perf.when_ready(changed, function() ran[#ran + 1] = "changed" end)
  vim.api.nvim_buf_delete(gone, { force = true })
  vim.bo[changed].filetype = "query"
  tick()
  eq(ran, {})
end)

test("Treesitter highlighting starts after the first screen, regex syntax until then", function()
  local perf = require("util.perf")
  vim.cmd("syntax on")
  local buf = buffer("vim", { "let x = 1" })
  perf.highlight(buf)
  perf.highlight(buf) -- FileType fires again when lazy.nvim replays it for plugins loaded on LazyFile
  eq(vim.treesitter.highlighter.active[buf], nil)
  eq(vim.bo[buf].syntax, "vim")
  local start, starts = vim.treesitter.start, 0
  vim.treesitter.start = function(...) starts = starts + 1 return start(...) end
  tick()
  vim.treesitter.start = start
  eq(starts, 1)
  assert(vim.treesitter.highlighter.active[buf], "highlighter not started")
  eq(vim.bo[buf].syntax, "")
  local other = buffer("vim", { "let y = 2" })
  perf.highlight(other)
  assert(vim.treesitter.highlighter.active[other], "second buffer must start at once")
end)

test("snacks quickfile paints with regex syntax (no Treesitter before the first screen)", function()
  local exclude = spec("ui", "folke/snacks.nvim").opts.quickfile.exclude
  for _, lang in ipairs({ "c", "lua", "vim", "latex" }) do
    assert(vim.tbl_contains(exclude, lang), lang .. " not excluded")
  end
end)

test("nvim-treesitter spec hands highlighting to util.perf", function()
  local s = spec("ui", "nvim-treesitter/nvim-treesitter")
  local opts = { highlight = { enable = true } }
  _G.LazyVim = { treesitter = { have = function() return true end } }
  s.opts(nil, opts)
  _G.LazyVim = nil
  eq(opts.highlight.enable, false)
  eq(#vim.api.nvim_get_autocmds({ group = "bender_ts_highlight", event = "FileType" }), 1)
  vim.api.nvim_del_augroup_by_name("bender_ts_highlight")
end)

test("Treesitter folds are flat until the language is ready, then recomputed", function()
  local perf = require("util.perf")
  _G.LazyVim = {
    treesitter = {
      _queries = {},
      have_query = function() end,
      foldexpr = function() return vim.v.lnum == 1 and ">1" or "=" end,
    },
  }
  perf.setup()
  local buf = buffer("markdown", { "a", "b", "c" })
  vim.api.nvim_set_current_buf(buf)
  vim.wo.foldexpr = "v:lua.LazyVim.treesitter.foldexpr()"
  vim.wo.foldmethod = "expr"
  eq(vim.fn.foldlevel(1), 0)
  tick()
  eq(vim.fn.foldlevel(1), 1)
  _G.LazyVim = nil
end)

test("rainbow delimiters attach once the language is ready", function()
  local attached = {}
  package.loaded["rainbow-delimiters.lib"] = { attach = function(b) attached[#attached + 1] = b end }
  local condition = spec("ui", "HiPhish/rainbow-delimiters.nvim").opts.condition
  local buf = buffer("vimdoc", { "*tag*" })
  eq(condition(buf), false)
  tick()
  eq(attached, { buf })
  local other = buffer("vimdoc", { "*x*" })
  eq(condition(other), true)
  package.loaded["rainbow-delimiters.lib"] = nil
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
