local builtin = require('telescope.builtin')
require('telescope').setup {
	extensions = {
		fzy_native = {
			override_generic_sorter = false,
			override_file_sorter = true,
		}
	}
}
