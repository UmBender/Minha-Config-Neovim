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
