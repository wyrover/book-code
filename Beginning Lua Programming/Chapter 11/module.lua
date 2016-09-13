io.output("testmod.lua")
io.write [[module(...)

Str = "Hello from " .. (...)
]]

io.close()
io.output(io.stdout)
package.path = "?.lua"
require("testmod")
io.write(testmod.Str, "\n")
