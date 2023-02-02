vim.g.mapleader = " "

-- Atalho para operacoes do proprio editor <Control> + comando
vim.keymap.set('n', '<C-s>',":w<CR>",{}) 	--Ctrl + s para salvar
vim.keymap.set('n', '<C-n>',":tabnew<CR>")  --Cria um arquivo novo

-- Atalho para navegacao e busca de arquivos <shift> + comando
local telescope = require('telescope.builtin')
vim.keymap.set('n', '<S-f>',telescope.find_files)


vim.keymap.set('n', '<S-l>',':BufferNext<CR>')      --Anda para as abas para direita
vim.keymap.set('n', '<S-h>',':BufferPrevious<CR>')  --Anda para as abas para esquerda

vim.keymap.set('n', '<S-e>',vim.cmd.NvimTreeFocus)  --Abre o NVimTree





