local map = function(lhs, rhs, desc)
  vim.keymap.set("n", lhs, rhs, { buffer = true, desc = desc })
end
local cp = require("util.cp")

local ok, wk = pcall(require, "which-key")
if ok then
  wk.add({ { "<leader>r", group = "run", icon = " ", buffer = 0 } })
end

-- run the package (tests: <leader>t, neotest; debug: <leader>d, delve)
map("<leader>rr", function()
  cp.run_go()
end, "Run Package")
map("<leader>ri", function()
  vim.ui.input({ prompt = "Arguments: " }, function(args)
    if args then
      cp.run_go(vim.split(args, "%s+", { trimempty = true }))
    end
  end)
end, "Run Package with Arguments")
