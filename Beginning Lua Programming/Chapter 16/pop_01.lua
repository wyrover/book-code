local socket = require("socket")

-- Run the specified configuration file in an empty environment. On success,
-- returns a table that contains all global variables that are assigned in
-- configuration file, otherwise (nil, error string).

local function Configure(CfgFileStr)
  local Gl, Cfg = getfenv(0), {}
  setfenv(0, Cfg)
  local Code, ErrStr = pcall(dofile, CfgFileStr)
  setfenv(0, Gl)
  return Code and Cfg or nil, ErrStr
end

-- Obtain POP server response. On success, return (string), otherwise (nil,
-- error string).

local function LclGet(PopHnd)
  local Str, ErrStr = PopHnd:receive()
  if Str then
    -- All POP responses begin with "+OK " or "-ERR "
    if string.sub(Str, 1, 1) == "+" then
      Str = string.sub(Str, 5)
    else
      ErrStr = string.sub(Str, 6)
      Str = nil
    end
  end
  -- return Ok, string.sub(Str, Ok and 5 or 6)
  return Str, ErrStr
end

-- Send a command to the POP server and obtain its response. Returns (string)
-- on success and (nil, error string) otherwise. The Multi argument is true if
-- the caller expects a multiline response from the POP server. If Multi is
-- true and command succeeds, returns (string, table) where table contains
-- returned data lines, otherwise (nil, error string).

local function LclTransact(PopHnd, SendStr, Multi)
  local Ok, Str, ErrStr, Tbl
  Ok, ErrStr = PopHnd:send(SendStr .. "\r\n")
  if Ok then
    Str, ErrStr = LclGet(PopHnd)
    if Str and Multi then
      Tbl = {}
      local DataStr
      while Multi and not ErrStr do
        DataStr, ErrStr = PopHnd:receive()
        if DataStr then
          if string.sub(DataStr, 1, 1) == "." then
            DataStr = string.sub(DataStr, 2)
            if DataStr == "" then
              Multi = false
            end
          end
          if Multi then
            Tbl[#Tbl + 1] = DataStr
          end
        end
      end
      if ErrStr then
        Str = nil
      else
        ErrStr = Tbl -- 2nd return value for Multi
      end
    end
  end
  return Str, ErrStr
end

-- Close the connection to the POP server. It is assumed that the server is in
-- a state to receive the "quit" command.

local function LclClose(PopHnd)
  LclTransact(PopHnd, "quit")
  PopHnd:close()
end

-- Connect to the POP server and authenticate user. If this function succeeds,
-- (client socket) is returned, otherwise (nil, error string) is returned.

local function LclOpen(Cfg)
  local Ok, Str, RetHnd, PopHnd, ErrStr
  PopHnd, ErrStr = socket.connect(Cfg.ServerStr, Cfg.ServerPort)
  if PopHnd then
    Str, ErrStr = LclGet(PopHnd)
    if Str then
      if LclTransact(PopHnd, "user " .. Cfg.UserStr) then
        if LclTransact(PopHnd, "pass " .. Cfg.PassStr) then
          RetHnd = PopHnd
        end
      end
      if not RetHnd then
        ErrStr = "Invalid username or password submitted"
      end
    end
    if not RetHnd then
      LclClose(PopHnd)
    end
  end
  return RetHnd, ErrStr
end

-- This function requests header information for each stored email message. On
-- success, it returns (header table), otherwise it returns (nil, error
-- string). Only the first line of each multiline value is saved.

local function LclHeaders(PopHnd)
  local Ok, Str, ErrStr, RetTbl, ListTbl

  Str, ListTbl = LclTransact(PopHnd, "list", true)
  if Str then
    RetTbl = {}
    for J, Str in ipairs(ListTbl) do
      local HdrTbl, MsgTbl = {}
      HdrTbl.size = tonumber(string.match(Str, "(%d+)$") or 0)
      Str, MsgTbl = LclTransact(PopHnd, "top " .. J .. " 1", true)
      if Str then
        for J, Str in ipairs(MsgTbl) do
          local KeyStr, ValStr = string.match(Str, "^([%a%-]+)%:%s*(.*)")
          if KeyStr then
            HdrTbl[string.lower(KeyStr)] = ValStr
          end
        end
        RetTbl[#RetTbl + 1] = HdrTbl
      else
        ErrStr = ErrStr and (ErrStr .. ", " .. MsgTbl) or MsgTbl
      end
    end
    if ErrStr then
      RetTbl = nil
    end
  else
    ErrStr = ListTbl
  end
  return RetTbl, ErrStr
end

local Cfg, PopHnd, ErrStr, Ok, HdrTbl, CfgFileStr

CfgFileStr = "cfg"
Cfg, ErrStr = Configure(CfgFileStr)
if Cfg then
  if type(Cfg.ServerStr) == "string" and type(Cfg.ServerPort) == "number" and
    type(Cfg.UserStr) == "string" and type(Cfg.PassStr) == "string" then
    io.write("Connecting to ", Cfg.ServerStr, "\n")
    PopHnd, ErrStr = LclOpen(Cfg)
    if PopHnd then
      io.write("Retrieving headers\n")
      HdrTbl, ErrStr = LclHeaders(PopHnd)
      if HdrTbl then
        for J, Rec in ipairs(HdrTbl) do
          io.write("--- Msg ", J, " ---\n",
            "  Subject: ", Rec.subject or "---", "\n",
            "  From:    ", Rec.from or "---", "\n",
            "  Date:    ", Rec.date or "---", "\n",
            "  Size:    ", Rec.size or "---", "\n")
        end
      end
      io.write("Disconnecting from ", Cfg.ServerStr, "\n")
      LclClose(PopHnd)
    end
  else
    io.write("Improper configuration: ", CfgFileStr, "\n")
  end
end
if ErrStr then
  io.write(ErrStr, "\n")
end
