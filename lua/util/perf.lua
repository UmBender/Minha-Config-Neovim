-- Performance tweaks that can't be expressed as plugin opts
-- (see tasks/Tasks/T-014 Performance.md and tasks/Tasks/T-022 Snappier editor.md)
local M = {}

--- Whether `lang` has a `query` file, without compiling it. Compiling a C++ query takes 30-250 ms.
---@return boolean
function M.have_query(lang, query)
  return #vim.treesitter.query.get_files(lang, query) > 0
end

-- Languages whose first buffer is past its first screen. Compiling the Treesitter queries of a language
-- (C++ highlights alone: ~260 ms) happens once per session, so only the first buffer of each one waits.
local ready = {} ---@type table<string, boolean>
local queued = {} ---@type table<string, fun()[]>

---@param buf integer
local function lang_of(buf)
  local ft = vim.bo[buf].filetype
  return vim.treesitter.language.get_lang(ft) or ft
end

--- Run `fn` for `buf` once its language is ready. The first buffer of a language waits until the screen
--- has been drawn (one tick, or the end of startup), so it shows up at once with Vim's regex syntax and
--- gets Treesitter highlights, folds and rainbow brackets right after.
---@param buf integer
---@param fn fun()
---@return boolean ready true when the language is already ready: `fn` is not queued, the caller runs it now
function M.when_ready(buf, fn)
  local lang = lang_of(buf)
  if ready[lang] then
    return true
  end
  local q = queued[lang]
  if not q then
    q = {}
    queued[lang] = q
    local function run()
      ready[lang], queued[lang] = true, nil
      for _, f in ipairs(q) do
        f()
      end
    end
    if vim.v.vim_did_enter == 1 then
      vim.schedule(run)
    else
      vim.api.nvim_create_autocmd("VimEnter", { once = true, callback = vim.schedule_wrap(run) })
    end
  end
  q[#q + 1] = function()
    if vim.api.nvim_buf_is_valid(buf) and lang_of(buf) == lang then
      fn()
    end
  end
  return false
end

--- Start Treesitter highlighting for `buf` (what LazyVim's FileType autocmd does), after the first screen.
---@param buf integer
function M.highlight(buf)
  local function start()
    -- FileType can fire twice for a buffer (lazy.nvim replays it for plugins loaded on LazyFile)
    if not vim.treesitter.highlighter.active[buf] then
      pcall(vim.treesitter.start, buf)
    end
  end
  if M.when_ready(buf, start) then
    start()
  end
end

--- Take over LazyVim's Treesitter highlighting (`opts.highlight.enable = false` on nvim-treesitter).
function M.setup_highlight()
  vim.api.nvim_create_autocmd("FileType", {
    group = vim.api.nvim_create_augroup("bender_ts_highlight", { clear = true }),
    callback = function(ev)
      if LazyVim.treesitter.have(ev.match, "highlights") then
        M.highlight(ev.buf)
      end
    end,
  })
end

--- LazyVim's `have_query` compiles the whole query (`query.get`) just to test that it exists, which
--- compiles textobjects/folds/indents on every startup even if they're never used.
--- Its `foldexpr` compiles the folds query on the first line it's asked about; until the language is
--- ready the buffer gets no folds, then they are recomputed.
function M.setup()
  local ts = LazyVim.treesitter
  ts.have_query = function(lang, query)
    local key = lang .. ":" .. query
    if ts._queries[key] == nil then
      ts._queries[key] = M.have_query(lang, query)
    end
    return ts._queries[key]
  end

  local foldexpr = ts.foldexpr
  local refold = {} ---@type table<integer, boolean>
  ts.foldexpr = function()
    local buf = vim.api.nvim_get_current_buf()
    if ready[lang_of(buf)] then
      return foldexpr()
    end
    if not refold[buf] then
      refold[buf] = true
      M.when_ready(buf, function()
        refold[buf] = nil
        for _, win in ipairs(vim.fn.win_findbuf(buf)) do
          -- setting 'foldmethod' recomputes the folds of the window
          vim.wo[win].foldmethod = vim.wo[win].foldmethod
        end
      end)
    end
    return "0"
  end
end

return M
