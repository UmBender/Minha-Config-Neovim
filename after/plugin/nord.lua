local highlights = require("nord").bufferline.highlights({
    italic = true,
    bold = true,
    fill = "#181c24"
})

if _G.colorscheme_select == "nord" then

	vim.cmd[[colorscheme nord]]
end
