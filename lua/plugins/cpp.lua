return {
  -- clangd tuned for single-file competitive programming
  {
    "neovim/nvim-lspconfig",
    opts = {
      servers = {
        clangd = {
          cmd = {
            "clangd",
            "--background-index",
            "--header-insertion=never", -- bits/stdc++.h covers everything
            "--completion-style=detailed",
            "--function-arg-placeholders=1",
            "--fallback-style=llvm",
          },
          init_options = {
            -- used when there is no compile_commands.json (i.e. a lone .cpp file)
            fallbackFlags = { "-std=c++20", "-DLOCAL" },
          },
        },
      },
    },
  },

  -- clang-format: project .clang-format if any, otherwise the one shipped with this config
  {
    "mason-org/mason.nvim",
    opts = { ensure_installed = { "clang-format" } },
  },
  {
    "stevearc/conform.nvim",
    opts = {
      formatters_by_ft = {
        c = { "clang-format" },
        cpp = { "clang-format" },
      },
      formatters = {
        ["clang-format"] = {
          prepend_args = function(_, ctx)
            if vim.fs.find(".clang-format", { upward = true, path = ctx.dirname })[1] then
              return { "--style=file" }
            end
            return { "--style=file:" .. vim.fn.stdpath("config") .. "/.clang-format" }
          end,
        },
      },
    },
  },

  -- test cases runner + Competitive Companion integration
  {
    "xeluxee/competitest.nvim",
    dependencies = { "MunifTanjim/nui.nvim" },
    cmd = "CompetiTest",
    opts = {
      compile_command = {
        cpp = {
          exec = "g++",
          args = { "-std=c++20", "-O2", "-Wall", "-Wextra", "-Wshadow", "-DLOCAL", "$(FNAME)", "-o", "$(FNOEXT)" },
        },
      },
      run_command = {
        cpp = { exec = "./$(FNOEXT)" },
      },
      maximum_time = 5000,
      template_file = {
        cpp = vim.fn.stdpath("config") .. "/templates/cp.cpp",
      },
      received_files_extension = "cpp",
    },
  },
}
