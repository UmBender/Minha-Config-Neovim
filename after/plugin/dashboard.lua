event = 'VimEnter'
function reconfig()
	vim.cmd("cd  ~/.config/nvim")
	vim.cmd("edit ~/.config/nvim/lua/bender/config.lua")
end

function renotes() 
	vim.cmd("cd ~/notes")
	vim.cmd("edit ~/notes/NOTES.md")
end
require('dashboard').setup {
	theme = 'doom',
	config = {
		header = {
			[[                                                  ]],
			[[                                                  ]],
			[[██████╗ ███████╗███╗   ██╗██████╗ ███████╗██████╗ ]],
			[[██╔══██╗██╔════╝████╗  ██║██╔══██╗██╔════╝██╔══██╗]],
			[[██████╔╝█████╗  ██╔██╗ ██║██║  ██║█████╗  ██████╔╝]],
			[[██╔══██╗██╔══╝  ██║╚██╗██║██║  ██║██╔══╝  ██╔══██╗]],
			[[██████╔╝███████╗██║ ╚████║██████╔╝███████╗██║  ██║]],
			[[╚═════╝ ╚══════╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚═╝  ╚═╝]],
			[[              ██╗   ██╗██╗███╗   ███╗             ]],
			[[              ██║   ██║██║████╗ ████║             ]],
			[[              ██║   ██║██║██╔████╔██║             ]],
			[[              ╚██╗ ██╔╝██║██║╚██╔╝██║             ]],
			[[               ╚████╔╝ ██║██║ ╚═╝ ██║             ]],
			[[                ╚═══╝  ╚═╝╚═╝     ╚═╝             ]],
			[[                                                  ]],
			[[                                                  ]],
		},
		center = {
			{
				icon = '🗎 ',
				icon_hl = 'Title',
				desc = 'Find File                               ',
				desc_hl = 'String',
				key = 'f',
				key_hl = 'Number',
				key_format = ' %s', -- remove default surrounding `[]`
				action = 'Telescope find_files'
			},
			{
				icon = '🗋 ',
				icon_hl = 'Title',
				desc = 'New File',
				desc_hl = 'String',
				key = 'n',
				key_hl = 'Number',
				key_format = ' %s', -- remove default surrounding `[]`
				action = 'tabnew'
			},
			{
				icon = '🔍',
				icon_hl = 'Title',
				desc = 'Find Text',
				desc_hl = 'String',
				key = 'g',
				key_hl = 'Number',
				key_format = ' %s', -- remove default surrounding `[]`
				action = 'Telescope grep_string'
			},
			{
				icon = '⚙ ',
				icon_hl = 'Title',
				desc = 'Configuration',
				desc_hl = 'String',
				key = 'c',
				key_hl = 'Number',
				key_format = ' %s', -- remove default surrounding `[]`
				action = reconfig
			},
			{
				icon = '🗏 ',
				icon_hl = 'Title',
				desc = 'Notes',
				desc_hl = 'String',
				key = 'e',
				key_hl = 'Number',
				key_format = ' %s', -- remove default surrounding `[]`
				action =	renotes 
			},
			{
				icon = '✕ ',
				icon_hl = 'Title',
				desc = 'Close',
				desc_hl = 'String',
				key = 'q',
				key_hl = 'Number',
				key_format = ' %s', -- remove default surrounding `[]`
				action = 'q!'
			},
		},

		week_header = {
			enable = false,
		},
		footer = {
			[[  ]],
			[[16]],

		},

	},
}

-- font ansi Shadow link: https://textkool.com/pt/ascii-art-generator
