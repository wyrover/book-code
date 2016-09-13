require "luacurl"
require "show"

local FileStr = "test"
local SiteStr = "http://www.lua.org/"

local function LclWrite(FileHnd, BufStr)
  FileHnd:write(BufStr)
  return string.len(BufStr)
end

local function LclProgress(UserPrm, DownTotal, DownNow, UpTotal, UpNow)
     print(DownTotal, DownNow, UpTotal, UpNow)
end

local Hnd = curl.new()
if Hnd then
  local FileHnd = io.open(FileStr, "wb")
  if FileHnd then
    Hnd:setopt(curl.OPT_WRITEFUNCTION, LclWrite)
    Hnd:setopt(curl.OPT_WRITEDATA, FileHnd)
    Hnd:setopt(curl.OPT_PROGRESSFUNCTION, LclProgress)
    Hnd:setopt(curl.OPT_NOPROGRESS, false)
    Hnd:setopt(curl.OPT_BUFFERSIZE, 5000)
    Hnd:setopt(curl.OPT_HTTPHEADER, "Connection: Keep-Alive",
      "Accept-Language: en-us")
    Hnd:setopt(curl.OPT_URL, SiteStr)
    Hnd:setopt(curl.OPT_CONNECTTIMEOUT, 15)
    local R = { Hnd:perform() }
    ObjectShow(R, "perform")
    local Ok, ErrStr, RetCode = Hnd:perform()
    if Ok then
      io.write("Resource successfully saved as ", FileStr, "\n")
    else
      io.write("Error: ", ErrStr, " (", RetCode, ")\n")
    end
    FileHnd:close()
  else
    io.write("Error opening ", FileStr, "\n")
  end
  Hnd:close()
else
  io.write("Error instantiating curl object.\n")
end
