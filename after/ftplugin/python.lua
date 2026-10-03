local map = function(lhs, rhs, desc)
  vim.keymap.set("n", lhs, rhs, { buffer = true, desc = desc })
end
local cp = require("util.cp")

local ok, wk = pcall(require, "which-key")
if ok then
  wk.add({ { "<leader>r", group = "competitive", icon = "󰙨 ", buffer = 0 } })
end

-- run / templates / library
map("<leader>rr", function()
  cp.run_py()
end, "Run")
map("<leader>ri", function()
  vim.ui.input({ prompt = "Arguments: " }, function(args)
    if args then
      cp.run_py(vim.split(args, "%s+", { trimempty = true }))
    end
  end)
end, "Run with Arguments")
map("<leader>rn", function()
  cp.pick_py_template()
end, "Insert Template")
map("<leader>rl", function()
  require("util.lib").python.pick()
end, "Insert from Library")
map("<leader>rf", function()
  require("util.fold").toggle(0)
end, "Toggle Template Folds")

-- library templates (`# Title:` .. `# End:`) open collapsed
require("util.fold").attach(0)

-- test cases (CompetiTest)
map("<leader>rt", "<cmd>CompetiTest run<cr>", "Run Testcases")
map("<leader>ru", "<cmd>CompetiTest show_ui<cr>", "Show Testcases UI")
map("<leader>ra", "<cmd>CompetiTest add_testcase<cr>", "Add Testcase")
map("<leader>re", "<cmd>CompetiTest edit_testcase<cr>", "Edit Testcase")
map("<leader>rx", "<cmd>CompetiTest delete_testcase<cr>", "Delete Testcase")
