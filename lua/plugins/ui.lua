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
      -- smooth scrolling makes every <C-d>, <C-u>, G and search jump wait for an animation
      scroll = { enabled = false },
      -- rainbow indent guides (colors defined in colorscheme.lua)
      indent = {
        animate = { enabled = false }, -- the scope guide appears at once instead of being drawn
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

  -- smear cursor (LazyVim extra), tuned to keep up with fast movement
  {
    "sphamba/smear-cursor.nvim",
    opts = {
      stiffness = 0.8,
      trailing_stiffness = 0.6,
      stiffness_insert_mode = 0.7,
      trailing_stiffness_insert_mode = 0.7,
      damping = 0.95,
      damping_insert_mode = 0.95,
      distance_stop_animating = 0.5, -- stop as soon as the tail is half a cell away
      time_interval = 7, -- ~140 fps instead of 60
    },
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
