

local function fileExists(path)
    local f = io.open(path, "r")
    if f ~= nil then
        io.close(f)
        return true
    else
        return false
    end
end

-- Tests if a directory "path" exists, returns true/false
local function dirExists(path)
  if type(path)~="string" then return false end
  attr = lfs.attributes(path)
  if attr ~= nil and attr.mode == "directory" then
    return true
  else
    return false
  end
end

function split_url(url)
   assert(type(url) == "string")
   
   local protocol, pathname = url:match("^([^:]*)://(.*)")
   if not protocol then
      protocol = "file"
      pathname = url
   end
   return protocol, pathname
end


function normalizePath(name)
   local protocol, pathname = split_url(name)
   pathname = pathname:gsub("\\", "/"):gsub("(.)/*$", "%1"):gsub("//", "/")
   if protocol ~= "file" then pathname = protocol .."://"..pathname end
   return pathname
end

function base_name(pathname)
   assert(type(pathname) == "string")

   local base = pathname:gsub("[/\\]*$", ""):match(".*[/\\]([^/\\]*)")
   return base or pathname
end

function dir_name(pathname)
   assert(type(pathname) == "string")
   return (pathname:gsub("/*$", ""):match("(.*)[/]+[^/]*")) or ""
end


return {
    fileExists = fileExists,
    dirExists = dirExists,
    normalizePath = normalizePath,
    base_name = base_name,
    dir_name = dir_name
}
