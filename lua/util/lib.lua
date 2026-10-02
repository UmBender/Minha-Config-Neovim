-- Template libraries: listing, dependency resolution, insertion and a picker.
-- `require("util.lib")` is the C++ one (lib/<area>/<name>.cpp); `.python` is pylib/<area>/<name>.py.
-- Both use the same header format, with the language's line comment.

---@class cp.LibConfig
---@field root string     directory of <area>/<name>.<ext> files
---@field ext string      "cpp", "py"
---@field comment string  line comment that starts the header lines: "//", "#"
---@field anchors string[] insert above the first line matching one of these (in order), else below the cursor
---@field title string    picker title

---@param cfg cp.LibConfig
local function new(cfg)
  local M = vim.deepcopy(cfg)

  --- Parse the leading `// Key: value` comment block. Indented `//   ...` lines continue the previous key.
  ---@return table<string, string>
  local function parse_header(file)
    local meta, key = {}, nil
    local c = vim.pesc(M.comment)
    for line in io.lines(file) do
      if not line:match("^" .. c) then
        break
      end
      local k, v = line:match("^" .. c .. " (%w+):%s?(.*)$")
      if k then
        key = k
        meta[k] = meta[k] and (meta[k] .. "\n" .. v) or v
      elseif key then
        meta[key] = meta[key] .. "\n" .. vim.trim(line:sub(#M.comment + 1))
      end
    end
    return meta
  end

  ---@class cp.LibItem
  ---@field id string        "area/name", or "area/name.variant" for a variant
  ---@field file string
  ---@field title string
  ---@field description string
  ---@field usage string
  ---@field verify? string
  ---@field requires string[]
  ---@field variant? string  "edu" or a common use ("sum", "add-min", ...); nil for the normal template
  ---@field variants cp.LibItem[] (structures only) educational first, then common uses by name
  ---@field search string    (structures only) text matched by the picker, variants included

  local function read_item(path)
    local file = M.root .. "/" .. path
    local meta = parse_header(file)
    local id = path:gsub("%." .. M.ext .. "$", "")
    return {
      id = id,
      file = file,
      title = meta.Title or path,
      description = vim.trim(meta.Description or ""),
      usage = meta.Usage or "",
      verify = meta.Verify,
      requires = vim.split(meta.Requires or "", "[,%s]+", { trimempty = true }),
      variant = id:match("^[^.]+%.(.+)$"),
    }
  end

  local function all_items()
    local items = {}
    for path, type in vim.fs.dir(M.root, { depth = 3 }) do
      if type == "file" and path:match("%." .. M.ext .. "$") then
        items[#items + 1] = read_item(path)
      end
    end
    return items
  end

  --- Structures (normal templates), each with its variants (`<name>.edu.cpp`, `<name>.<use>.cpp`).
  ---@return cp.LibItem[]
  function M.list()
    local items, by_id = {}, {}
    local variants = {}
    for _, item in ipairs(all_items()) do
      if item.variant then
        variants[#variants + 1] = item
      else
        item.variants = {}
        items[#items + 1], by_id[item.id] = item, item
      end
    end
    for _, v in ipairs(variants) do
      local base = by_id[v.id:match("^[^.]+")]
      if base then
        table.insert(base.variants, v)
      end
    end
    for _, item in ipairs(items) do
      table.sort(item.variants, function(a, b)
        if (a.variant == "edu") ~= (b.variant == "edu") then
          return a.variant == "edu"
        end
        return a.variant < b.variant
      end)
      local parts = { item.id, item.title, item.description }
      for _, v in ipairs(item.variants) do
        vim.list_extend(parts, { v.id, v.title, v.description })
      end
      item.search = table.concat(parts, " ")
    end
    table.sort(items, function(a, b)
      return a.id < b.id
    end)
    return items
  end

  --- Second menu for a structure: normal, educational, then common uses. Empty without variants.
  ---@return {id: string, label: string, item: cp.LibItem}[]
  function M.menu(item)
    if #item.variants == 0 then
      return {}
    end
    local entries = { { id = item.id, label = "normal", item = item } }
    for _, v in ipairs(item.variants) do
      entries[#entries + 1] = { id = v.id, label = v.variant == "edu" and "educational" or v.variant, item = v }
    end
    return entries
  end

  local function get(id)
    for _, item in ipairs(all_items()) do
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

  --- 0-based line to insert at: above the first anchor (`void solve`, then `int main`), else below the cursor.
  local function insertion_line(buf, lines)
    for _, pattern in ipairs(M.anchors) do
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
      local title = line:match("^" .. vim.pesc(M.comment) .. " Title: (.*)$")
      if title then
        present[title] = true
      end
    end

    local chunk, inserted = {}, {}
    for _, dep in ipairs(M.resolve(id)) do
      local item = get(dep)
      if not present[item.title] then
        vim.list_extend(chunk, vim.fn.readfile(item.file))
        vim.list_extend(chunk, { M.comment .. " End: " .. item.title, "" }) -- closes the template's fold (util.fold)
        inserted[#inserted + 1] = dep
      end
    end
    if #chunk > 0 then
      local at = insertion_line(buf, lines)
      vim.api.nvim_buf_set_lines(buf, at, at, false, chunk)
    end
    return inserted
  end

  local function notify_insert(id, buf)
    local inserted = M.insert(id, buf)
    require("util.fold").close(
      buf,
      vim.tbl_map(function(dep)
        return get(dep).title
      end, inserted)
    )
    local msg = #inserted > 0 and ("Inserted " .. table.concat(inserted, ", ")) or (id .. " is already in the file")
    vim.notify(msg, vim.log.levels.INFO, { title = M.title })
  end

  --- Second step: pick a variant of `item` (normal, educational, common uses).
  local function pick_variant(item, buf)
    Snacks.picker({
      title = item.title,
      items = vim.tbl_map(function(e)
        return {
          id = e.id,
          label = e.label,
          file = e.item.file,
          description = e.item.description,
          text = e.label .. " " .. e.item.description,
        }
      end, M.menu(item)),
      preview = "file",
      format = function(e)
        return {
          { ("%-14s"):format(e.label), "SnacksPickerFile" },
          { " " .. e.description, "SnacksPickerComment" },
        }
      end,
      confirm = function(picker, e)
        picker:close()
        if e then
          notify_insert(e.id, buf)
        end
      end,
    })
  end

  --- Fuzzy picker over the library; structures with variants open a second menu, the rest insert directly.
  function M.pick()
    local buf = vim.api.nvim_get_current_buf()
    local items = vim.tbl_map(function(item)
      return vim.tbl_extend("force", item, { text = item.search })
    end, M.list())
    Snacks.picker({
      title = M.title,
      items = items,
      preview = "file",
      format = function(item)
        return {
          { ("%-28s"):format(item.title), "SnacksPickerFile" },
          { " " .. item.id, "SnacksPickerDir" },
          { #item.variants > 0 and ("  +" .. #item.variants .. " variants") or "", "SnacksPickerComment" },
        }
      end,
      confirm = function(picker, item)
        picker:close()
        if not item then
          return
        end
        if #item.variants > 0 then
          vim.schedule(function()
            pick_variant(item, buf)
          end)
        else
          notify_insert(item.id, buf)
        end
      end,
    })
  end

  return M
end

local config = vim.fn.stdpath("config")
local M = new({
  root = config .. "/lib",
  ext = "cpp",
  comment = "//",
  anchors = { "^void solve", "^int main" },
  title = "CP Library",
})
M.python = new({
  root = config .. "/pylib",
  ext = "py",
  comment = "#",
  anchors = { "^def solve", "^def gen", "^def main", "^if __name__" },
  title = "Python Library",
})
M.new = new

--- The library for a filetype, nil if there is none.
function M.for_filetype(ft)
  return ({ cpp = M, python = M.python })[ft]
end

return M
