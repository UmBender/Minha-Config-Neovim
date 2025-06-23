vim.opt.relativenumber = true
vim.opt.termguicolors  = true
vim.opt.shiftwidth     = 4
vim.opt.tabstop        = 4
vim.opt.scrolloff      = 10

vim.o.termguicolors    = true
vim.o.cursorline       = true
vim.o.number           = true
vim.o.mouse            = nil
_G.colorscheme_select  = "moonfly"
vim.api.nvim_create_autocmd("FileType", {
	pattern = "php",
	callback = function()
		vim.bo.expandtab = true
		vim.bo.shiftwidth = 4
		vim.bo.tabstop = 4
		vim.bo.softtabstop = 4
		vim.bo.autoindent = true
		vim.bo.smartindent = true
	end,
})
vim.opt.clipboard = 'unnamedplus'
