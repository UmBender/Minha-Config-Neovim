-- Tests for the precompiled header in lua/util/cp.lua. Run: nvim --headless --clean -l tests/nvim/cp_test.lua
local root = vim.fn.fnamemodify(debug.getinfo(1, "S").source:sub(2), ":p:h:h:h")
package.path = root .. "/lua/?.lua;" .. root .. "/lua/?/init.lua;" .. package.path

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
local function exists(path)
  return vim.uv.fs_stat(path) ~= nil
end

test("the PCH lives in the cache, one dir per flag set", function()
  eq(cp.pch_root, vim.fn.stdpath("cache") .. "/pch")
  local a, b = cp.pch_dir(cp.flags), cp.pch_dir(cp.debug_flags)
  eq(a, cp.pch_dir(vim.list_slice(cp.flags)))
  assert(a ~= b, "release and debug share a PCH")
  eq(vim.fs.dirname(a), cp.pch_root)
  assert(cp.pch_dir({ "-O2" }) ~= cp.pch_dir({ "-O0" }), "key ignores the flags")
end)

test("pch_args adds the dir to the include path and warns on a stale PCH", function()
  eq(cp.pch_args(cp.flags), { "-I", cp.pch_dir(cp.flags), "-Winvalid-pch" })
end)

test("compile_cmd uses the flags and the PCH of the mode", function()
  local want = vim.list_extend({ "g++" }, cp.flags)
  vim.list_extend(want, cp.pch_args(cp.flags))
  vim.list_extend(want, { "/x/a.cpp", "-o", "/x/a" })
  eq(cp.compile_cmd("/x/a.cpp", "/x/a"), want)
  local dbg = cp.compile_cmd("/x/a.cpp", "/x/a", true)
  assert(vim.tbl_contains(dbg, "-D_GLIBCXX_DEBUG"), "debug flags")
  assert(vim.tbl_contains(dbg, cp.pch_dir(cp.debug_flags)), "debug PCH")
end)

test("CompetiTest compiles with the same flags and PCH", function()
  local spec
  for _, s in ipairs(dofile(root .. "/lua/plugins/cpp.lua")) do
    if s[1] == "xeluxee/competitest.nvim" then
      spec = s
    end
  end
  assert(type(spec.opts) == "function", "opts must be a function (no g++ calls at startup)")
  local cc = spec.opts(spec, {}).compile_command.cpp
  eq(cc.exec, "g++")
  local want = vim.list_extend(vim.list_slice(cp.flags), cp.pch_args(cp.flags))
  eq(cc.args, vim.list_extend(want, { "$(FNAME)", "-o", "$(FNOEXT)" }))
end)

-- the rest builds a real PCH in a temp cache
local tmp = vim.fn.tempname()
cp.pch_root = tmp
cp.flags = { "-std=c++20", "-DLOCAL" }

test("prune_pch keeps only the current release and debug PCHs", function()
  local keep, keep_dbg, stale = cp.pch_dir(cp.flags), cp.pch_dir(cp.debug_flags), tmp .. "/0123456789abcdef"
  for _, d in ipairs({ keep, keep_dbg, stale }) do
    vim.fn.mkdir(d .. "/bits", "p")
  end
  cp.prune_pch()
  assert(exists(keep) and exists(keep_dbg), "current PCH removed")
  assert(not exists(stale), "stale PCH kept")
  vim.fn.delete(keep, "rf")
  vim.fn.delete(keep_dbg, "rf")
end)

test("build_pch builds a PCH that g++ uses, once per flag set", function()
  local gch = cp.pch_dir(cp.flags) .. "/bits/stdc++.h.gch"
  local results = {}
  assert(cp.build_pch(cp.flags, function(ok)
    results[#results + 1] = ok
  end) == true, "first call starts a build")
  assert(cp.build_pch(cp.flags, function(ok)
    results[#results + 1] = ok
  end) == false, "second call while building starts another build")
  assert(
    vim.wait(90000, function()
      return #results == 2
    end, 100),
    "build did not finish"
  )
  eq(results, { true, true })
  assert(exists(gch), "no .gch at " .. gch)
  eq(vim.fn.glob(vim.fs.dirname(gch) .. "/*.tmp*", false, true), {})

  local done
  assert(cp.build_pch(cp.flags, function(ok)
    done = ok
  end) == false, "rebuilt an existing PCH")
  eq(done, true)

  local src = tmp .. "/a.cpp"
  vim.fn.writefile({ "#include <bits/stdc++.h>", "int main() {}" }, src)
  local cmd = cp.compile_cmd(src, tmp .. "/a")
  vim.list_extend(cmd, { "-H", "-fsyntax-only" })
  local res = vim.system(cmd, { text = true }):wait()
  eq(res.code, 0)
  assert(res.stderr:find("! " .. gch, 1, true), "PCH not used:\n" .. res.stderr:sub(1, 400))
end)

vim.fn.delete(tmp, "rf")
if failures > 0 then
  os.exit(1)
end
print("cp_test: all passed")
