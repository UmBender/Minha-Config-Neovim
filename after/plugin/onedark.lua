require('onedark').setup {
    style = 'darker',
	transparent = true,
	term_colors = true,
	code_style  = {
		comments = 'italic',
		keyword  = 'bold',
		functions= 'italic,bold',
		strings  = 'italic',
		-- variables= ''
	},
	diagnostics  = {
		undercurl = false,
	},
	colors = {
		    bright_orange = "#ffff00",
	},
	highlighths = {
			["@string"] = {fg = '#ffff00', sp = '#ffff00'},
	},
}
if colorscheme_select == "onedark" then
	
	require('onedark').load()
end

