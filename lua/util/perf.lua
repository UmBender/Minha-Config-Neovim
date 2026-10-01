-- Performance tweaks that can't be expressed as plugin opts (see tasks/Tasks/T-014 Performance.md)
local M = {}

--- Whether `lang` has a `query` file, without compiling it. Compiling a C++ query takes 30-250 ms.
---@return boolean
function M.have_query(lang, query)
  return #vim.treesitter.query.get_files(lang, query) > 0
end

--- LazyVim's `have_query` compiles the whole query (`query.get`) just to test that it exists, which
--- compiles textobjects/folds/indents on every startup even if they're never used.
function M.setup()
  local ts = LazyVim.treesitter
  ts.have_query = function(lang, query)
    local key = lang .. ":" .. query
    if ts._queries[key] == nil then
      ts._queries[key] = M.have_query(lang, query)
    end
    return ts._queries[key]
  end
end

return M
