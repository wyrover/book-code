return function()
	local t = {}

	local out = {
		push = function(v)
			table.insert(t, v)
		end,
		pop = function()
			return table.remove(t)
		end,
		empty = function()
			return not (#t > 0)
		end,
	}
	return out
end
