-- C++ template library (lib/<area>/<name>.cpp): listing, dependency resolution, insertion and a picker
local M = {}

M.root = vim.fn.stdpath("config") .. "/lib"

--- Parse the leading `// Key: value` comment block. Indented `//   ...` lines continue the previous key.
---@return table<string, string>
local function parse_header(file)
  local meta, key = {}, nil
  for line in io.lines(file) do
    if not line:match("^//") then
      break
    end
    local k, v = line:match("^// (%w+):%s?(.*)$")
    if k then
      key = k
      meta[k] = meta[k] and (meta[k] .. "\n" .. v) or v
    elseif key then
      meta[key] = meta[key] .. "\n" .. vim.trim(line:sub(3))
    end
  end
  return meta
end

---@class cp.LibItem
---@field id string        "area/name"
---@field file string
---@field title string
---@field description string
---@field usage string
---@field verify? string
---@field requires string[]

---@return cp.LibItem[]
function M.list()
  local items = {}
  for path, type in vim.fs.dir(M.root, { depth = 3 }) do
    if type == "file" and path:match("%.cpp$") then
      local file = M.root .. "/" .. path
      local meta = parse_header(file)
      items[#items + 1] = {
        id = path:gsub("%.cpp$", ""),
        file = file,
        title = meta.Title or path,
        description = vim.trim(meta.Description or ""),
        usage = meta.Usage or "",
        verify = meta.Verify,
        requires = vim.split(meta.Requires or "", "[,%s]+", { trimempty = true }),
      }
    end
  end
  table.sort(items, function(a, b) return a.id < b.id end)
  return items
end

local function get(id)
  for _, item in ipairs(M.list()) do
    if item.id == id then
      return item
    end
  end
  error("unknown template: " .. id)
end

--- Template ids to insert for `id`, dependencies first, each once.
---@return string[]
function M.resolve(id, seen, order)
  seen, order = seen or {}, order or {}
  if not seen[id] then
    seen[id] = true
    for _, dep in ipairs(get(id).requires) do
      M.resolve(dep, seen, order)
    end
    order[#order + 1] = id
  end
  return order
end

--- 0-based line to insert at: above `void solve`, else above `int main`, else below the cursor.
local function insertion_line(buf, lines)
  for _, pattern in ipairs({ "^void solve", "^int main" }) do
    for i, line in ipairs(lines) do
      if line:match(pattern) then
        return i - 1
      end
    end
  end
  local win = vim.fn.bufwinid(buf)
  return win ~= -1 and vim.api.nvim_win_get_cursor(win)[1] or #lines
end

--- Insert a template (and missing dependencies) into `buf`.
---@return string[] ids actually inserted
function M.insert(id, buf)
  buf = buf or vim.api.nvim_get_current_buf()
  local lines = vim.api.nvim_buf_get_lines(buf, 0, -1, false)
  local present = {}
  for _, line in ipairs(lines) do
    local title = line:match("^// Title: (.*)$")
    if title then
      present[title] = true
    end
  end

  local chunk, inserted = {}, {}
  for _, dep in ipairs(M.resolve(id)) do
    local item = get(dep)
    if not present[item.title] then
      vim.list_extend(chunk, vim.fn.readfile(item.file))
      chunk[#chunk + 1] = ""
      inserted[#inserted + 1] = dep
    end
  end
  if #chunk > 0 then
    local at = insertion_line(buf, lines)
    vim.api.nvim_buf_set_lines(buf, at, at, false, chunk)
  end
  return inserted
end

--- Fuzzy picker over the library; inserts the selection into the current buffer.
function M.pick()
  local buf = vim.api.nvim_get_current_buf()
  local items = vim.tbl_map(function(item)
    return vim.tbl_extend("force", item, { text = item.id .. " " .. item.title .. " " .. item.description })
  end, M.list())
  Snacks.picker({
    title = "CP Library",
    items = items,
    preview = "file",
    format = function(item)
      return {
        { ("%-28s"):format(item.title), "SnacksPickerFile" },
        { " " .. item.id, "SnacksPickerDir" },
      }
    end,
    confirm = function(picker, item)
      picker:close()
      if item then
        local inserted = M.insert(item.id, buf)
        local msg = #inserted > 0 and ("Inserted " .. table.concat(inserted, ", ")) or (item.id .. " is already in the file")
        vim.notify(msg, vim.log.levels.INFO, { title = "CP Library" })
      end
    end,
  })
end

return M
