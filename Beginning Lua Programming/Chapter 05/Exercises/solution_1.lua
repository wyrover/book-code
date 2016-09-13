-- Returns an array of Str's characters:
function StrToArr(Str)
  local Ret = {}
  for I = 1, #Str do
    Ret[I] = string.sub(Str, I, I)
  end
  return Ret
end
