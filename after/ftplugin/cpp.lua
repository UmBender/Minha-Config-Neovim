local map = function(lhs, rhs, desc)
  vim.keymap.set("n", lhs, rhs, { buffer = true, desc = desc })
end
local cp = require("util.cp")

local ok, wk = pcall(require, "which-key")
if ok then
  wk.add({ { "<leader>r", group = "competitive", icon = "󰙨 ", buffer = 0 } })
end

-- quick compile / run
map("<leader>rc", function() cp.compile() end, "Compile")
map("<leader>rr", function() cp.run() end, "Compile & Run")
map("<leader>rd", function() cp.run({ debug = true }) end, "Compile & Run (sanitizers)")
map("<leader>rn", function() cp.insert_template() end, "Insert Template")

-- test cases (CompetiTest)
map("<leader>rt", "<cmd>CompetiTest run<cr>", "Run Testcases")
map("<leader>rT", "<cmd>CompetiTest run_no_compile<cr>", "Run Testcases (no compile)")
map("<leader>ru", "<cmd>CompetiTest show_ui<cr>", "Show Testcases UI")
map("<leader>ra", "<cmd>CompetiTest add_testcase<cr>", "Add Testcase")
map("<leader>re", "<cmd>CompetiTest edit_testcase<cr>", "Edit Testcase")
map("<leader>rx", "<cmd>CompetiTest delete_testcase<cr>", "Delete Testcase")

-- Competitive Companion (browser extension)
map("<leader>rp", "<cmd>CompetiTest receive problem<cr>", "Receive Problem")
map("<leader>rP", "<cmd>CompetiTest receive contest<cr>", "Receive Contest")
map("<leader>rR", "<cmd>CompetiTest receive testcases<cr>", "Receive Testcases")
