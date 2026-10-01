-- Options are automatically loaded before lazy.nvim startup
-- Default options: https://github.com/LazyVim/LazyVim/blob/main/lua/lazyvim/config/options.lua
local opt = vim.opt

opt.shiftwidth = 4
opt.tabstop = 4
opt.softtabstop = 4
opt.scrolloff = 10
opt.clipboard = "unnamedplus"

-- the winbar (dropbar) already shows the code context; skip LazyVim's lualine copy of it
vim.g.trouble_lualine = false

if _G.LazyVim then
  require("util.perf").setup()
end
