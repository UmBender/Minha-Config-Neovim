compile_type = {
	["c"]= "gcc % -o %< && ./%<",
	["cpp"]= "g++ % -o %< && ./%<"
}
vim.keymap.set('n', '<C-y>',":FloatermNew --autoclose=0 python3 % <CR>")
-- vim.keymap.set('n', '<C-p>',":FloatermNew --autoclose=0 gcc % -o %< && ./%<<CR>")
vim.keymap.set('n', '<C-p>',":FloatermNew --autoclose=0 zsh ./build.sh <CR>")
-- vim.keymap.set('n', '<C-p>',":FloatermNew --autoclose=0 g++ % -o %< && ./%<<CR>")
vim.keymap.set('n', '<C-t>',":FloatermNew <CR>")
vim.keymap.set('t', '<C-t>', "exit <CR> <CR>")
