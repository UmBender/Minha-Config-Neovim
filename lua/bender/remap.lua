vim.g.mapleader = " "

-- Atalho para navegacao e busca de arquivos <shift> + comando
local telescope = require('telescope.builtin')
vim.keymap.set('n', '<leader>ff', telescope.find_files)
vim.keymap.set('n', '<space>t', telescope.grep_string)


vim.keymap.set('n', '<S-l>', ':BufferNext<CR>', { silent = true })     --Anda para as abas para direita
vim.keymap.set('n', '<S-h>', ':BufferPrevious<CR>', { silent = true }) --Anda para as abas para esquerda

vim.keymap.set('n', '<leader>e', vim.cmd.NvimTreeFocus)                --Abre o NVimTree

vim.keymap.set('n', 'K', vim.lsp.buf.hover, opts)
vim.keymap.set('n', 'gd', '<cmd>lua vim.lsp.buf.definition()<cr>', opts)
vim.keymap.set('n', 'gD', '<cmd>lua vim.lsp.buf.declaration()<cr>', opts)
vim.keymap.set('n', 'gi', '<cmd>lua vim.lsp.buf.implementation()<cr>', opts)
vim.keymap.set('n', 'go', '<cmd>lua vim.lsp.buf.type_definition()<cr>', opts)
vim.keymap.set('n', 'gr', '<cmd>lua vim.lsp.buf.references()<cr>', opts)
vim.keymap.set('n', '<C-k>', vim.lsp.buf.signature_help, opts)
vim.keymap.set('n', '<space>wa', vim.lsp.buf.add_workspace_folder, opts)
vim.keymap.set('n', '<space>wr', vim.lsp.buf.remove_workspace_folder, opts)
vim.keymap.set('n', '<space>wl', function()
	print(vim.inspect(vim.lsp.buf.list_workspace_folders()))
end, opts)
vim.keymap.set('n', '<space>D', vim.lsp.buf.type_definition, opts)
vim.keymap.set('n', '<space>rn', vim.lsp.buf.rename, opts)
vim.keymap.set({ 'n', 'v' }, '<space>ca', vim.lsp.buf.code_action, opts)
vim.keymap.set('n', '<space>fe', function()
	vim.lsp.buf.format { async = true }
end, opts)
vim.keymap.set('n', '<leader>s', ":LspZeroFormat<CR>:w<CR>", { silent = true }) --Ctrl + s para salvar
vim.keymap.set('n', '<leader>q', ":LspZeroFormat<CR>:wq<CR>", {silent = true})             --Ctrl + q para salvar e sair
vim.keymap.set('n', '<leader>n', ":tabnew<CR>")                               --Cria um arquivo novo
vim.keymap.set('n', '<leader>c', ":BufferClose<CR>")                          -- Fecha a aba atual
vim.keymap.set('n', '<leader>p', ":!./build.sh<CR>")                          -- Fecha a aba atual
