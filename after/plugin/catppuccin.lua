colorscheme_cat = {
	"latte",
	"frappe",
	"macchiato",
	"mocha"
}

option_color_cat =1


transparent_background_config = true
if colorscheme_cat[option_color_cat] == "latte" then
	transparent_background_config = false
end

require("catppuccin").setup({
	flavour = colorscheme_cat[option_color_cat], -- latte, frappe, macchiato, mocha
	-- background = { -- :h background
	--    light = "latte",
	--     dark = "mocha",
	-- },
	transparent_background = transparent_background_config,
	show_end_of_buffer = false, -- show the '~' characters after the end of buffers term_colors = true,
	dim_inactive = {
		enabled = true,
		shade = "dark",
		percentage = 1.0,
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
		variables = {"bold"},
		numbers = {},
		booleans = {},
		properties = {},
		types = {},
		operators = {},
	}, color_overrides = {},
	custom_highlights = {},
	integrations = {
		cmp = true,
		gitsigns = true,
		nvimtree = true,
		telescope = true,
		notify = true,
		mini = false,
		dashboard = true,
		indent_blankline = {
			enabled = true,
			colored_indent_levels = true,
		},
		mason = true,
        -- For more plugins integrations please scroll down (https://github.com/catppuccin/nvim#integrations)
    },
})

if _G.colorscheme_select == "catppuccin" then
	require("catppuccin").load();
end


