require('onedark').setup {
    style = 'darker',
	transparent = false,
	term_colors = true,
	code_style  = {
		comments = 'italic',
		keyword  = 'bold,underline',
		functions= 'bold',
		strings  = 'italic',
		variables= 'bold'
	},
	diagnostics  = {
		undercurl = false,
	},
	colors = {
		    bright_orange = "#ffff00",
	},
	highlighths = {
			["@string"] = {fg = '#ffff00', sp = '$cyan'},
	}
}
require('onedark').load()
