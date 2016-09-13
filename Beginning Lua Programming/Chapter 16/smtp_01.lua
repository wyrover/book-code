local smtp = require("socket.smtp")

local Recipient = {"<recipient_1@example.net>", "<recipient_2@example.net>"}

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

local BodyStr = [[
Now the hungry lion roars,
  And the wolf behowls the moon;
Whilst the heavy ploughman snores,
  All with weary task foredone.
]]

local Msg = {
  headers = {
    to = "Oberon <oberon@example.com> Titania <titania@example.com>",
    cc = "William Shakespeare <bard@globe.example.org>",
    subject = "Moonlight",
    from = "Puck <puck@example.com>",
  },
  body = BodyStr
}

local Packet = {}
local Code
local CfgFileStr = "smtp.cfg"
local Cfg, ErrStr = Configure(CfgFileStr)
if Cfg then
  if type(Cfg.ServerStr) == "string" and type(Cfg.ServerPort) == "number" and
    type(Cfg.UserStr) == "string" and type(Cfg.PassStr) == "string" then
    io.write("Connecting to ", Cfg.ServerStr, "\n")
    Packet.from = "<puck@example.com>"
    Packet.rcpt = Recipient
    Packet.source = smtp.message(Msg)
    Packet.server = Cfg.ServerStr
    Packet.port = Cfg.ServerPort
    Packet.password = Cfg.PassStr
    Packet.user = Cfg.UserStr
    Code, ErrStr = smtp.send(Packet)
    if Code then
      io.write("Successfully sent messages")
    end
  else
    ErrStr = "Improper configuration: " .. CfgFileStr
  end
end
if ErrStr then
  io.write(ErrStr, "\n")
end
