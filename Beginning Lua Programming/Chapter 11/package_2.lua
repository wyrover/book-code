io.output("testmod.lua")
io.write [[module(..., package.seeall)
local ModStr = (...)

function Hello()
  io.write("Hello from ", ModStr, "\n")
end
]]

io.close()
io.output(io.stdout)
package.path = "?.lua"
require("testmod")
testmod.Hello()
