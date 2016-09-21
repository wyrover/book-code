-- module1.lua

local var1 = 'ipsum'
local function local_function1()
  return 'lorem'
end

local function local_function2(self)
  return var1 .. self.var2
end
-- returns module interface
return {
  lorem = local_function1,
  ipsum = local_function2,
  var2 = 'sit',
}
