local lsp_zero = require('lsp-zero')
lsp_zero.preset('recommended')

lsp_zero.on_attach(function(client, bufnr)
	-- see :help lsp-zero-keybindings
	-- to learn the available actions
	lsp_zero.default_keymaps({ buffer = bufnr })
end)

lsp_zero.configure('rust_analyzer', {
	settings = {
		['rust-analyzer'] = {
			checkOnSave = true,
		},
	},
})

require('lspconfig').gleam.setup({})

require('mason').setup({})
require('mason-lspconfig').setup({
	ensure_installed = { "clangd", },
	handlers = {
		lsp_zero.default_setup,
		clangd = function()
			require('lspconfig').clangd.setup({
				single_file_support = true,
				on_attach = function(client, bufnr)
					print('hello tsserver')
				end,
				init_options = {
					fallbackFlags = { '--std=c++20' }
				},
			})
		end,
	},
})
