local ti = table.insert
local tr = table.remove

-- removes element from table by its value
local tr2 = function(t, v)
	for i=1,#t do
		if t[i]==v then
			tr(t, i)
			break
		end
	end
end

return function ()
	local t = {}

	-- a set of elements
	local set = {}
	-- a set of priorities paired with a elements
	local r_set = {}
	-- sorted list of priorities
	local keys = {}

	-- add element into storage, set its priority and sort keys
	-- k - element
	-- v - priority
	local function addKV(k, v)
       	set[k] = v
       	-- create a new list for this priority
		if not r_set[v] then
			ti(keys, v)
			table.sort(keys)
			local k0 = {k}
			r_set[v] = k0
			setmetatable(k0, {
				__mode = 'v'
			})
		-- add element into list for this priority
		else
			ti(r_set[v], k)
		end
	end

	-- remove element from storage and sort keys
	local remove = function(k)
       	local v = set[k]
       	local prioritySet = r_set[v]
       	tr2(prioritySet, k)
       	if #prioritySet < 1 then
       		tr2(keys, v)
			r_set[v] = nil
			table.sort(keys)
			set[k] = nil
       	end
	end; t.remove = remove

	-- returns an element with the lowest priority
	t.min = function()
		local priority = keys[1]
		if priority then
			return r_set[priority] or {}
		else
			return {}
		end
	end

	-- returns an element with the highest priority
	t.max = function()
		local priority = keys[#keys]
		if priority then
			return r_set[priority] or {}
		else
			return {}
		end
	end

	-- is this queue empty?
	t.empty = function()
		return #keys < 1
	end

	setmetatable(t, {
		__index = set,
		__newindex = function(t, k, v)
			if not set[k] then
				-- new item
				addKV(k, v)
			else
	        	-- existing item
	        	remove(k)
				addKV(k, v)
			end
		end,
	})

	return t
end
