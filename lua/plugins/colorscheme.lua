return {
  {
    "rebelot/kanagawa.nvim",
    priority = 1000,
    opts = {
      theme = "dragon",
      background = { dark = "dragon" },
      transparent = true,
      -- higher contrast palette, same as my terminal colors
      colors = {
        palette = {
          dragonRed = "#d16961", -- red
          dragonGreen2 = "#8aa86e", -- green
          dragonYellow = "#ceb680", -- yellow
          dragonBlue2 = "#7fa8bc", -- blue
          dragonPink = "#aa88ac", -- magenta
          dragonAqua = "#82b0ab", -- cyan
          oldWhite = "#d0c58b", -- white
          waveRed = "#ec6070", -- bright red
          dragonGreen = "#7bb57b", -- bright green
          carpYellow = "#ecc57e", -- bright yellow
          springBlue = "#75b8d3", -- bright blue
          springViolet1 = "#8e7eb5", -- bright magenta
          waveAqua2 = "#6db5a7", -- bright cyan
          dragonOrange = "#c28f6f", -- extended color 1
          dragonOrange2 = "#c4886f", -- extended color 2
        },
        theme = {
          all = {
            ui = {
              bg_gutter = "none", -- keep the gutter transparent too
              -- floats (explorer, pickers, hover) look like the editor
              float = { fg = "#c5c9c5", bg = "none", bg_border = "none" },
            },
          },
        },
      },
      overrides = function(colors)
        local p = colors.palette
        return {
          -- rainbow indent guides (see snacks.indent in ui.lua)
          RainbowRed = { fg = p.dragonRed },
          RainbowYellow = { fg = p.dragonYellow },
          RainbowBlue = { fg = p.dragonBlue2 },
          RainbowOrange = { fg = p.dragonOrange },
          RainbowGreen = { fg = p.dragonGreen2 },
          RainbowViolet = { fg = p.dragonPink },
          RainbowCyan = { fg = p.dragonAqua },
          -- separator between the explorer and splits, visible without a background
          WinSeparator = { fg = p.dragonBlack5 },
          -- underline the word under the cursor and its references
          LspReferenceText = { underline = true },
          LspReferenceRead = { underline = true },
          LspReferenceWrite = { underline = true },
        }
      end,
    },
  },
  {
    "LazyVim/LazyVim",
    opts = { colorscheme = "kanagawa-dragon" },
  },
  -- colorschemes bundled with LazyVim that aren't used
  { "folke/tokyonight.nvim", enabled = false },
  { "catppuccin/nvim", name = "catppuccin", enabled = false },
}
