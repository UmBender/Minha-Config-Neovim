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
-- require('onedark').load()
--
require("catppuccin").setup({
    flavour = "macchiato", -- latte, frappe, macchiato, mocha
    -- background = { -- :h background
    --    light = "latte",
    --     dark = "mocha",
    -- },
    transparent_background = false,
    show_end_of_buffer = false, -- show the '~' characters after the end of buffers
    term_colors = false,
    dim_inactive = {
        enabled = false,
        shade = "dark",
        percentage = 0.15,
    },
    no_italic = false, -- Force no italic
    no_bold = false, -- Force no bold
    styles = {
        comments = { "italic" },
        conditionals = { "italic" },
        loops = {},
        functions = {"italic,bold"},
        keywords = { "bold" },
        strings = {"italic"},
        variables = {},
        numbers = {},
        booleans = {},
        properties = {},
        types = {},
        operators = {},
    },
    color_overrides = {},
    custom_highlights = {},
    integrations = {
        cmp = true,
        gitsigns = true,
        nvimtree = true,
        telescope = true,
        notify = false,
        mini = false,
        -- For more plugins integrations please scroll down (https://github.com/catppuccin/nvim#integrations)
    },
})

-- setup must be called before loading
require("catppuccin").load();
-- vim.cmd.colorscheme "catppuccin-mocha"
