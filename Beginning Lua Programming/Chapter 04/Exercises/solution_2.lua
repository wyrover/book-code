CompAll = function(A, B)
  if type(A) ~= type(B) then
    -- If they're of different types, sort them by type:
    A, B = type(A), type(B)
  elseif type(A) ~= "number" and type(A) ~= "string" then
    -- If they're something other than numbers or strings,
    -- sort them by their tostring representation:
    A, B = tostring(A), tostring(B)
  end
  return A < B
end
