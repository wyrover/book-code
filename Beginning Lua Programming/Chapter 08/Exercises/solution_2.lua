-- Makes Tbl read-only (and returns it):
function Protect(Tbl)
  -- Tbl will be used as a proxy table; transfer all its
  -- pairs to OrigTbl:
  local OrigTbl = {}
  for Key, Val in pairs(Tbl) do
    OrigTbl[Key] = Val
    Tbl[Key] = nil -- Clear the soon-to-be proxy.
  end
  -- __newindex prevents writes to the table; __index allows
  -- reads from the (original) table; __metatable prevents
  -- metatable trickery:
  local ProtectMeta = {
    __newindex = function()
      error("attempt to assign to a protected table")
    end,
    __index = OrigTbl,
    __metatable = true}
  -- As well as setting Tbl's metatable, return it (for
  -- convenience):
  return setmetatable(Tbl, ProtectMeta)
end
