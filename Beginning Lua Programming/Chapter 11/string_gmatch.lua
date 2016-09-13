for Str in string.gmatch("CamelbackName", "%u%l+") do
  io.write(Str, "\n")
end
