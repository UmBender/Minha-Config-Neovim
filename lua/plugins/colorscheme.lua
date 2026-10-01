return {
  {
    "ellisonleao/gruvbox.nvim",
    priority = 1000,
    opts = {
      transparent_mode = true,
      overrides = {
        -- rainbow indent guides (see snacks.indent in ui.lua)
        RainbowRed = { fg = "#E06C75" },
        RainbowYellow = { fg = "#E5C07B" },
        RainbowBlue = { fg = "#61AFEF" },
        RainbowOrange = { fg = "#D19A66" },
        RainbowGreen = { fg = "#98C379" },
        RainbowViolet = { fg = "#C678DD" },
        RainbowCyan = { fg = "#56B6C2" },
        -- underline the word under the cursor and its references
        LspReferenceText = { underline = true },
        LspReferenceRead = { underline = true },
        LspReferenceWrite = { underline = true },
      },
    },
  },
  {
    "LazyVim/LazyVim",
    opts = { colorscheme = "gruvbox" },
  },
}
