--local lsp = require('lsp-zero').preset({})
require'lspconfig'.pyright.setup{}
require'lspconfig'.tsserver.setup{}
require'lspconfig'.rust_analyzer.setup{}
require'lspconfig'.clojure_lsp.setup{}
require'lspconfig'.ccls.setup{}
require'lspconfig'.gopls.setup{}
require'lspconfig'.marksman.setup{}
require'lspconfig'.solargraph.setup{}
require'lspconfig'.html.setup{
	cmd = { "html-languageserver", "--stdio"},
	on_attach = on_attach,
	capabilities = capabilities,
	flags = lsp_flags
}
require'lspconfig'.cssls.setup{
	cmd = { "css-languageserver", "--stdio"},
	on_attach = on_attach,
	capabilities = capabilities,
	flags = lsp_flags
}
require'lspconfig'.jsonls.setup{
	cmd = { "json-languageserver", "--stdio"},
	on_attach = on_attach,
	capabilities = capabilities,
	flags = lsp_flags
}





--lsp.on_attach(function(client, bufnr)
--  lsp.default_keymaps({buffer = bufnr})
--end)

-- When you don't have mason.nvim installed
-- You'll need to list the servers installed in your system
--lsp.setup_servers({'tsserver', 'eslint', 'rust_analyzer'})

-- (Optional) Configure lua language server for neovim
--require('lspconfig').lua_ls.setup(lsp.nvim_lua_ls())

--lsp.setup()
