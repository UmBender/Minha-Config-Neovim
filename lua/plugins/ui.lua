return {
  -- start screen
  {
    "folke/snacks.nvim",
    opts = {
      dashboard = {
        preset = {
          header = [[
██████╗ ███████╗███╗   ██╗██████╗ ███████╗██████╗
██╔══██╗██╔════╝████╗  ██║██╔══██╗██╔════╝██╔══██╗
██████╔╝█████╗  ██╔██╗ ██║██║  ██║█████╗  ██████╔╝
██╔══██╗██╔══╝  ██║╚██╗██║██║  ██║██╔══╝  ██╔══██╗
██████╔╝███████╗██║ ╚████║██████╔╝███████╗██║  ██║
╚═════╝ ╚══════╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚═╝  ╚═╝
              ██╗   ██╗██╗███╗   ███╗
              ██║   ██║██║████╗ ████║
              ██║   ██║██║██╔████╔██║
              ╚██╗ ██╔╝██║██║╚██╔╝██║
               ╚████╔╝ ██║██║ ╚═╝ ██║
                ╚═══╝  ╚═╝╚═╝     ╚═╝]],
          -- stylua: ignore
          keys = {
            { icon = " ", key = "f", desc = "Find File", action = ":lua Snacks.dashboard.pick('files')" },
            { icon = " ", key = "n", desc = "New File", action = ":ene | startinsert" },
            { icon = " ", key = "g", desc = "Find Text", action = ":lua Snacks.dashboard.pick('live_grep')" },
            { icon = " ", key = "r", desc = "Recent Files", action = ":lua Snacks.dashboard.pick('oldfiles')" },
            { icon = " ", key = "c", desc = "Config", action = ":lua Snacks.dashboard.pick('files', {cwd = vim.fn.stdpath('config')})" },
            { icon = " ", key = "e", desc = "Notes", action = function() vim.cmd("cd ~/notes | edit ~/notes/NOTES.md") end },
            { icon = " ", key = "s", desc = "Restore Session", section = "session" },
            { icon = "󰒲 ", key = "l", desc = "Lazy", action = ":Lazy", enabled = package.loaded.lazy ~= nil },
            { icon = " ", key = "q", desc = "Quit", action = ":qa" },
          },
        },
      },
      -- rainbow indent guides (colors defined in colorscheme.lua)
      indent = {
        indent = {
          hl = {
            "RainbowRed",
            "RainbowBlue",
            "RainbowOrange",
            "RainbowGreen",
            "RainbowViolet",
            "RainbowCyan",
            "RainbowYellow",
          },
        },
      },
    },
  },

  -- "bubbles" statusline
  {
    "nvim-lualine/lualine.nvim",
    opts = function(_, opts)
      opts.options.component_separators = "|"
      opts.options.section_separators = { left = "", right = "" }
      opts.sections.lualine_a = {
        { "mode", separator = { left = "" }, right_padding = 2 },
      }
      opts.sections.lualine_z = {
        { "location", separator = { right = "" }, left_padding = 2 },
      }
    end,
  },

  -- rainbow (), [], {} with the Kanagawa palette (groups in colorscheme.lua)
  {
    "HiPhish/rainbow-delimiters.nvim",
    event = "LazyFile",
    main = "rainbow-delimiters.setup",
    opts = {},
  },

  -- compact rounded diagnostics next to the code, replacing the default virtual text
  {
    "rachartier/tiny-inline-diagnostic.nvim",
    event = "LspAttach",
    priority = 1000,
    opts = { preset = "modern" },
  },
  {
    "neovim/nvim-lspconfig",
    opts = { diagnostics = { virtual_text = false } },
  },

  -- winbar with file path and code context
  {
    "Bekaboo/dropbar.nvim",
    event = "LazyFile",
    keys = {
      { "<leader>;", function() require("dropbar.api").pick() end, desc = "Pick Winbar Symbol" },
    },
    opts = {},
  },
}
