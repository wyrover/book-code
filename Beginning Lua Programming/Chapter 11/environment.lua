Str = "Global"

function Sandbox()
  print(getfenv(), Str)
end

setfenv(Sandbox, {print = print, getfenv = getfenv, Str = "Sandbox"})
Sandbox()
print(getfenv(), Str)
