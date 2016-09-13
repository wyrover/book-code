-- Sample echo server for testing with inetd and cousin xinetd

local Str = "Lua echo server opening / " .. os.date("%T")
while Str and Str ~= "" and Str ~= "\r" do
  io.write(Str, "\r\n")
  io.flush()
  Str = io.read()
end
io.write("Lua echo server closing / ", os.date("%T"), "\r\n")
