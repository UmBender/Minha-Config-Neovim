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
