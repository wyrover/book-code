local ModStr = "modload"
print("Main, before require", package.loaded[ModStr])
local Val = require(ModStr)
print("Main, after 1st require", package.loaded[ModStr], Val)
Val = require(ModStr)
print("Main, after 2nd require", package.loaded[ModStr], Val)
package.loaded[ModStr] = nil
Val = require(ModStr)
print("Main, after reset", package.loaded[ModStr], Val)
