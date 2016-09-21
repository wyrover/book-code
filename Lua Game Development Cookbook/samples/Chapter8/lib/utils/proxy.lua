table.proxy = function(t)
  -- newproxy is not available in Lua 5.2+ !!!
  if type(newproxy)=='function' then
    assert(type(t) == 'table', '1st argument should be a table')
    
    -- create a new proxy object
    local p = newproxy(true)
    local p_mt = getmetatable(p)
		
    -- set GC meta-method for proxy object
    p_mt.__gc = function()
      local mt = getmetatable(t)
      local gc_fn = mt.__gc
      if type(gc_fn)=='function' then
        gc_fn(t)
      end
    end

    -- store proxy object in a metatable
    local mt = getmetatable(t) or {}; mt.__proxy = p; setmetatable(t, mt)
  end
end
