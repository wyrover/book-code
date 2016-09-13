Meta = {
  -- Returns a reverse-order copy of an array:
  __unm = function(Arr)
    local Ret = {}
    for I = #Arr, 1, -1 do
      Ret[#Ret + 1] = Arr[I]
    end
    return Ret
  end}
