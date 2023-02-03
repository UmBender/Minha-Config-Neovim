return require('packer').startup(function(use)
    -- Instalacao do packer
    use 'wbthomason/packer.nvim'

    --Pacote instalado do telescope
    use 'nvim-lua/plenary.nvim'
    use { 'nvim-telescope/telescope.nvim', tag = '0.1.0' }

    --Instalacao do treesitter
    use { 'nvim-treesitter/nvim-treesitter', run = ':TSUpdate' }

	--Instalacao de icones para apresentacao
	use 'nvim-tree/nvim-web-devicons'

	--Barra lateral para navegar entre arquivos
    use {
        'nvim-tree/nvim-tree.lua',
        requires = {
            'nvim-tree/nvim-web-devicons',
        },
        tag = 'nightly'
    }

	--Servidor para linguagem rust
	use 'rust-lang/rust.vim'

	--Instalacao do mason(Gerenciador de pacotes), que gerencia e instala
	-- LSP
	-- DAP
	-- linters
	-- formatters
	-- TODO Configurar
	use 'williamboman/mason.nvim'

	-- Para usar o nvim como um language server, para injetar diagnosticos de LSP, acoes de codigo
	-- e mais, via LUA
	-- TODO Configurar
	use 'jose-elias-alvarez/null-ls.nvim'

	-- Debugger adapter protocol, uma implementacao para o nvim
	-- Inicie um aplicativo para depurar
    -- Anexe a aplicativos em execução e depure-os
    -- Definir pontos de interrupção e percorrer o código
    -- Inspecione o estado do aplicativo
	-- TODO Configurar
	use 'mfussenegger/nvim-dap'

	-- TODO Configurar atalhos e configuracao
    use {
        'VonHeikemen/lsp-zero.nvim',
        requires = {
            -- LSP Support
            { 'neovim/nvim-lspconfig' },
            { 'williamboman/mason.nvim' },
            { 'williamboman/mason-lspconfig.nvim' },

            -- Autocompletion
            { 'hrsh7th/nvim-cmp' },
            { 'hrsh7th/cmp-buffer' },
            { 'hrsh7th/cmp-path' },
            { 'saadparwaiz1/cmp_luasnip' },
            { 'hrsh7th/cmp-nvim-lsp' },
            { 'hrsh7th/cmp-nvim-lua' },

            -- Snippets
            { 'L3MON4D3/LuaSnip' },
            { 'rafamadriz/friendly-snippets' },
        }
    }

    -- O meu esquema de cores
    use 'navarasu/onedark.nvim'

	-- Uma tabline para o nvim 
	-- TODO Configurar
	use { 'romgrk/barbar.nvim', wants = 'nvim-web-devicons' }

	-- Um status line para o nvim
	-- TODO Configurar
	use {
		'nvim-lualine/lualine.nvim',
		requires = { 'kyazdani42/nvim-web-devicons', opt = true }
	}

	-- Adiciona um status do LSP ao status line
	-- TODO Configurar
	use 'nvim-lua/lsp-status.nvim'

	-- Adiciona uma enfase nos espacos em branco para mostrar qual seria o escopo
	-- TODO Configurar
	use "lukas-reineke/indent-blankline.nvim"

	-- Adiciona uma enfase na palavra e linha que o cursor está
	-- TODO Configurar
    use "yamatsum/nvim-cursorline"

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
	use "mawkler/modicator.nvim"

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
	use 'akinsho/toggleterm.nvim'

	--Plugin que adiciona uma status line que mostra o atual contexto do codigo
	--TODO Configurar
	use { "SmiteshP/nvim-navic",requires = "neovim/nvim-lspconfig"}


	use 'fgheng/winbar.nvim'

	-- Plugin para gerar a lista que mostra os diagnosticos, referencias, quickfix ...
	-- TODO Configurar
	use 'folke/trouble.nvim'

	-- Plugin que lista possiveis atalhos para comandos que voce comecou a digitar
	-- TODO Configurar
	use 'folke/which-key.nvim'


end)









