
require("lfs")

fs = {}

function fs.size(path)
    return io.filesize(path)
end

function fs.isdir(path)
    local mode = lfs.attributes(path, "mode")
    return mode == "directory"
end

function fs.isfile(path)
    return not fs.isdir(path)
end

function fs.rename(oldname, newname)
    return os.rename(oldname, newname)
end


function fs.read(path)
    local file = io.open(path, "rb")
    if file then
        local content = file:read("*all")
        io.close(file)
        return content
    end
    return nil
end

function fs.write(path, content, mode)
    mode = mode or "w+b"
    local file = io.open(path, mode)
    if file then
        if file:write(content) == nil then
            return false
        end
        io.close(file)
        return true
    else
        return false
    end
end

function fs.remove(path)
    local mode = lfs.attributes(path, "mode")
    if mode == "directory" then
        local dirPath = path.."/"
        for file in lfs.dir(dirPath) do
            if file ~= "." and file ~= ".." then
                local f = dirPath..file
                fs.remove(f)
            end
        end
        lfs.rmdir(path)
    else
        os.remove(path)
    end
end

function fs.list(path)
    local files={}
    local dirs={}
    local function lll(dir)
        for file in lfs.dir(dir) do
            if file== "." or file == ".." then
            else
                local p = dir..'/'..file
                local mode = lfs.attributes(p,"mode")
                if mode == "directory" then
                    lll(p)
                    table.insert(dirs,p)
                else
                    table.insert(files,p)
                end
            end
        end
    end
    local mode = lfs.attributes(path,"mode")
    if mode ~= "directory" then
        return
    end
    lll(path)
    return dirs,files
end

function fs.split(input, delimiter)
    input = tostring(input)
    delimiter = tostring(delimiter)
    if (delimiter=='') then return false end
    local pos,arr = 0, {}
    -- for each divider found
    for st,sp in function() return string.find(input, delimiter, pos, true) end do
        table.insert(arr, string.sub(input, pos, st - 1))
        pos = sp + 1
    end
    table.insert(arr, string.sub(input, pos))
    return arr
end

--创建目录
function fs.mkdir(path)
    if not fs.exists(path) then
        return lfs.mkdir(path)
    end
    return true
end

function fs.dofile(path)
    local fileData = CZHelperFunc:getFileData(path)
    local fun = loadstring(fileData)
    local ret, r = pcall(fun)
    if ret then
        return r
    end
end

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

function isDir(filePath)
    local isDir = false

    local attr = lfs.attributes(filePath)
    if (type(attr) == "table") and attr.mode == "directory" then
        isDir = true
    end

    return isDir
end

function isFile(filePath)
    local attr = lfs.attributes(filePath)
    if (type(attr) == "table") and attr.mode == "file" then
        return true
    end

    return false
end

function exist(filePath)
    local attr = lfs.attributes(filePath)
    if attr then
        return true
    else
        return false
    end
end

function fixDirSuffix(filePath)
    if isDir(filePath) then
        if not string.find(filePath, "/$") then
            return filePath .. "/"
        end
    end

    return filePath
end

-- 查找指定文件中与正则相匹配的字符串
function searchTargetStr(fileName, regex)
    local file = io.open(fileName, "r+")
    local contentList = {}
    if file then
        local buffer = file:read("*a")
        string.gsub(buffer, regex, function(...)
            contentList[#contentList + 1] = select(1, ...)
        end)

        io.close(file)
    end

    return contentList
end


function replaceTargetStr(fileName, match, replaceFunc)
    local file = io.open(fileName, "r+")
    local contentList = {}
    if file then
        local buffer = file:read("*a")

        buffer = string.gsub(buffer, match, function(...)
            local result = replaceFunc(select(1, ...))
            return result
        end)

        io.close(file)

        local file = io.open(fileName, "w+")
        file:write(buffer)
        io.close(file)
    end

    return contentList
end

function ensureDir(path)
    local part, index = path:match("([/\\]?[^/\\]+[/\\])()")
    local subPath = ""

    while part do
        subPath = subPath .. part
        lfs.mkdir(subPath)
        part, index = path:match("([^/\\]+[/\\])()", index)
    end
end


return {
    fileExists = fileExists,
    dirExists = dirExists,
    normalizePath = normalizePath,
    base_name = base_name,
    dir_name = dir_name,
    isDir = isDir,
    isFile = isFile
}
