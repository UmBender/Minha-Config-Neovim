local lsp_zero = require('lsp-zero')
lsp_zero.preset('recommended')

lsp_zero.on_attach(function(client, bufnr)
	-- see :help lsp-zero-keybindings
	-- to learn the available actions
	lsp_zero.default_keymaps({ buffer = bufnr })
	vim.diagnostic.config({
		virtual_text = true, -- Show inline warnings
		signs = true,  -- Show gutter icons
		update_in_insert = false,
	})
end)

vim.lsp.config('rust_analyzer', {
	settings = {
		['rust-analyzer'] = {
			checkOnSave = true,
		},
	},
})

vim.lsp.config('zls', {
	settings = {
		zls = {
			enable_formatter = true, -- Enable built-in formatter
			format_on_save = true, -- Format on save
			ignore_unused = true, -- Don't warn about unused variables

			-- Diagnostics
			warn_style = true,                 -- Style warnings
			enable_semantic_tokens = true,     -- Enhanced highlighting
			enable_inlay_hints = true,         -- Show inline hints
			inlay_hints_show_builtin = true,   -- Show builtin fn hints
			inlay_hints_exclude_single_argument = true, -- Cleaner hints
			inlay_hints_hide_redundant_param_names = true,
			check_for_duplicate_fields = true, -- Struct field checks

			-- Completion
			include_at_in_builtins = true, -- @import completion
			skip_std_references = false, -- Include std lib in completions
			max_detail_length = 1048576, -- Full completion docs
			use_comptime_interpreter = true, -- Better comptime analysis

			-- Advanced
			enable_ast_check_diagnostics = true, -- AST-based diagnostics
			enable_import_embedfile_argument_completions = true,
			enable_snippets = true,     -- Enable code snippets
			force_on_save_behavior = false, -- Don't force save behavior
			enable_global_analysis = true, -- Cross-file analysis
			preferred_config_path = ".zls.json", -- Project-specific config
			record_session = true,      -- Session recording		}
		}
	},
	capabilities = {
		experimental = {
			serverStatusNotification = true -- Show server status
		}
	},
	-- Optional: Add keymaps only for Zig files
	on_attach = function(client, bufnr)
		lsp_zero.default_keymaps({ buffer = bufnr })
		-- Custom keymaps
		vim.keymap.set('n', 'gr', '<cmd>Telescope lsp_references<cr>', { buffer = true, desc = "References" })
		vim.keymap.set('n', '<leader>db', "<cmd>lua require('dap').toggle_breakpoint()<cr>",
			{ buffer = true, desc = "Toggle Breakpoint" })
		vim.keymap.set('n', '<leader>dc', "<cmd>lua require('dap').continue()<cr>", { buffer = true, desc = "Continue" })
	end
})



require('mason').setup({})
require('mason-lspconfig').setup({
	ensure_installed = { "clangd", "zls", },
	handlers = {
		lsp_zero.default_setup,
		clangd = function()
			require('lspconfig').clangd.setup({
				single_file_support = true,
				on_attach = function(client, bufnr)
					print('hello tsserver')
				end,
				init_options = {
					fallbackFlags = { '--std=c++20' }
				},
			})
		end,
	},
})

require('mason-nvim-dap').setup({
	ensure_installed = { 'codelldb' }, -- Debugger for Zig
	automatic_installation = true,
})

local dap = require('dap')
dap.adapters.codelldb = {
	type = 'server',
	port = '${port}',
	executable = {
		command = vim.fn.stdpath('data') .. '/mason/bin/codelldb',
		args = { '--port', '${port}' },
	}
}

dap.configurations.zig = {
	{
		name = 'Debug',
		type = 'codelldb',
		request = 'launch',
		program = function()
			return vim.fn.input('Path to executable: ', vim.fn.getcwd() .. '/', 'file')
		end,
		cwd = '${workspaceFolder}',
		stopOnEntry = false,
	},
}

lsp_zero.setup()
