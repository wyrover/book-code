-- module2.lua
local M = function()
  local instance
  local var1 = 'ipsum'
  instance = {
    var2 = 'sit',
    lorem = function()
      return 'lorem'
    end,
    ipsum = function(self)
      return var1 .. self.var2
    end,
  }
  return instance
end

return M
