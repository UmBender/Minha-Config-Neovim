-- Autocmds are automatically loaded on the VeryLazy event
-- Default autocmds: https://github.com/LazyVim/LazyVim/blob/main/lua/lazyvim/config/autocmds.lua

-- New .cpp files start from the competitive programming template
vim.api.nvim_create_autocmd("BufNewFile", {
  group = vim.api.nvim_create_augroup("bender_cpp_template", { clear = true }),
  pattern = "*.cpp",
  callback = function(ev)
    require("util.cp").insert_template(ev.buf)
  end,
})

-- New .py files get a template only when the name says which (gen*.py, stress*.py, brute*.py, p42.py, ...)
vim.api.nvim_create_autocmd("BufNewFile", {
  group = vim.api.nvim_create_augroup("bender_py_template", { clear = true }),
  pattern = "*.py",
  callback = function(ev)
    local cp = require("util.cp")
    local kind = cp.py_template_for(ev.file)
    if kind then
      cp.insert_py_template(ev.buf, kind)
    end
  end,
})
