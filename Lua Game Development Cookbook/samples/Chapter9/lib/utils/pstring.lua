do
	local mt = debug.getmetatable("")
	mt['__mod'] = function(a, b)
		assert(type(a) == "string" and type(b)=="table")
		return a:format(unpack(b))
	end
end
