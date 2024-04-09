return require('packer').startup(function(use)
	-- Instalacao do packer
	use { 'wbthomason/packer.nvim' }
	use {
		'VonHeikemen/lsp-zero.nvim',
		branch = 'v3.x',
		requires = {
			--- Uncomment these if you want to manage LSP servers from neovim
			{ 'williamboman/mason.nvim' },
			{ 'williamboman/mason-lspconfig.nvim' },

			-- LSP Support
			{ 'neovim/nvim-lspconfig' },
			-- Autocompletion
			{ 'hrsh7th/nvim-cmp' },
			{ 'hrsh7th/cmp-nvim-lsp' },
			{ 'L3MON4D3/LuaSnip',
				tag = "v2.*", -- Replace <CurrentMajor> by the latest released major (first number of latest release)
				-- install jsregexp (optional!:).
				run = "make install_jsregexp" },
		}
	}
	use { "rafamadriz/friendly-snippets" }
	use { 'L3MON4D3/LuaSnip' }

	-- auto-completion engine
	-- nvim-cmp completion sources
	use({ "hrsh7th/cmp-nvim-lsp", after = "nvim-cmp" })
	use({ "hrsh7th/cmp-path", after = "nvim-cmp" })
	use({ "hrsh7th/cmp-buffer", after = "nvim-cmp" })
	use({ "hrsh7th/cmp-omni", after = "nvim-cmp" })

	use { 'saadparwaiz1/cmp_luasnip' }

	--Pacote instalado do telescope
	use { 'nvim-lua/plenary.nvim' }
	use { 'nvim-telescope/telescope.nvim', tag = '0.1.5' }

	--Instalacao do treesitter
	use { 'nvim-treesitter/nvim-treesitter', run = ':TSUpdate' }

	--Instalacao de icones para apresentacao
	use { 'nvim-tree/nvim-web-devicons' }

	--Barra lateral para navegar entre arquivos
	use {
		'nvim-tree/nvim-tree.lua',
		requires = {
			'nvim-tree/nvim-web-devicons',
		},
		 
	}

	--Servidor para linguagem rust

	--Instalacao do mason(Gerenciador de pacotes), que gerencia e instala
	-- LSP
	-- DAP
	-- linters
	-- formatters
	-- TODO Configurar
	-- use 'williamboman/mason.nvim'

	-- Para usar o nvim como um language server, para injetar diagnosticos de LSP, acoes de codigo
	-- e mais, via LUA
	-- TODO Configurar
	use { 'jose-elias-alvarez/null-ls.nvim' }

	-- Debugger adapter protocol, uma implementacao para o nvim
	-- Inicie um aplicativo para depurar
	-- Anexe a aplicativos em execução e depure-os
	-- Definir pontos de interrupção e percorrer o código
	-- Inspecione o estado do aplicativo
	-- TODO Configurar
	use { 'mfussenegger/nvim-dap' }

	-- TODO Configurar atalhos e configuracao
	-- O meu esquema de cores
	use { 'navarasu/onedark.nvim' }

	-- Novo esquema de cores teste
	use { 'catppuccin/nvim' }

	-- Uma tabline para o nvim
	-- TODO Configurar
	use { 'romgrk/barbar.nvim', wants = 'nvim-web-devicons' }
	-- Adiciona um status do LSP ao status line
	-- TODO Configurar
	-- use { 'nvim-lua/lsp-status.nvim' }

	-- Adiciona uma enfase nos espacos em branco para mostrar qual seria o escopo
	-- TODO Configurar
	use { "lukas-reineke/indent-blankline.nvim" }

	-- Adiciona uma enfase na palavra e linha que o cursor está
	-- TODO Configurar
	use { "yamatsum/nvim-cursorline" }

	-- Adiciona um live server, para facilitar o desenvolvimento em pagina web
	-- Tem que ter o npm
	-- TODO Configurar
	use {
		"aurum77/live-server.nvim",
		run = function()
			require "live_server.util".install()
		end,
		cmd = { "LiveServer", "LiveServerStart", "LiveServerStop" },
	}

	-- Muda o foreground color com base no atual Vim Mode
	-- TODO Configurar
	use { "mawkler/modicator.nvim" }

	-- Plugin para facilitar comentar multiplas linhas adicionando atalhos
	-- TODO Configurar
	use {
		'numToStr/Comment.nvim',
		config = function()
			require('Comment').setup()
		end
	}

	--Plugin para utilizar um terminal dentro do nvim como maior facilidade
	--TODO Configurar
	use { 'akinsho/toggleterm.nvim' }

	--Plugin que adiciona uma status line que mostra o atual contexto do codigo
	--TODO Configurar
	-- use { "SmiteshP/nvim-navic", requires = "neovim/nvim-lspconfig" }


	use { 'fgheng/winbar.nvim' }

	-- Plugin para gerar a lista que mostra os diagnosticos, referencias, quickfix ...
	-- TODO Configurar
	use { 'folke/trouble.nvim' }

	-- Plugin que lista possiveis atalhos para comandos que voce comecou a digitar
	-- TODO Configurar
	use { 'folke/which-key.nvim' }


	-- Um status line para o nvim
	-- TODO Configurar
	use {
		'nvim-lualine/lualine.nvim',
		requires = { 'kyazdani42/nvim-web-devicons', opt = true }
	}



	-- Plugin que cria uma página inicial para o nvim
	-- TODO Configurar
	use { 'glepnir/dashboard-nvim' }

	-- Plugin para criacao de configuração de notificações
	-- TODO Configurar
	use { 'rcarriga/nvim-notify' }

	-- Plugin para criar um terminal flutuante
	-- TODO Configurar
	use { 'voldikss/vim-floaterm' }


	use { 'shaunsingh/nord.nvim' }



	-- Integration with git
	use {
		'tanvirtin/vgit.nvim',
		requires = {
			'nvim-lua/plenary.nvim'
		}
	}

	-- Highlight color for neovim
	use { 'norcalli/nvim-colorizer.lua' }

	-- New colorscheme
	use { 'nyoom-engineering/oxocarbon.nvim' }


	use { 'nvim-telescope/telescope-fzf-native.nvim', run = 'make' }

	use { 'terrortylor/nvim-comment' }

	-- For latex
	use { 'lervag/vimtex' }
end)
