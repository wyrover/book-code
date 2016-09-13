io.output("a.lua")
io.write('return {Str = "Module a (file)."}')
io.close()
io.output("b.lua")
io.write('return {Str = "Module b (file)."}')
io.close()
io.output("c.lua")
io.write('return {Str = "Module c (file)."}')
io.close()
io.output(io.stdout)

package.cpath = ""
package.path = "./?.lua"

function package.preload.a()
  return {Str = "Module a."}
end

package.loaded.b = {Str = "Module b."}

local function Report(ModStr)
  io.write('package.loaded["', ModStr, '"] = ',
    tostring(package.loaded[ModStr]), '\n')
end

Report("a")
Report("b")
Report("c")
local A = require("a")
local B = require("b")
local C = require("c")
io.write("A: ", A.Str, "\nB: ", B.Str, "\nC: ", C.Str, "\n")
Report("a")
Report("b")
Report("c")
