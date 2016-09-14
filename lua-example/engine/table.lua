local mathe = require("engine.math")

local tinsert = table.insert
local tremove = table.remove
local next = next
local setmetatable = setmetatable

local function forEach(tbl, func)
	for k, v in next, tbl do
		func(k, v)
	end
end

local function fill(tbl, val, startIndex, endIndex)
	tbl = tbl or {}
	if startIndex then
		if startIndex < 0 then
			startIndex = len - startIndex
		end
		
		if endIndex and endIndex < 0 then
			endIndex = len - endIndex
		end
	
		for i = startIndex, endIndex or #tbl do
			tbl[i] = val
		end
		return tbl
	end
	for k, v in next, tbl do
		tbl[k] = val
	end
	return tbl
end

local function isSequential(tbl)
	local numericKeys = 0
	local tblLen = #tbl
	for k, v in next, tbl do
		numericKeys = numericKeys + 1
		if numericKeys > tblLen then
			return false
		end
	end
	return true
end

local function assertSequential(tbl)
	assert(isSequential(tbl), "Table is not sequential")
end	

local function filter(tbl, func--[[
, createNew]])
	--local target = createNew and {} or tbl
	if isSequential(tbl) then
		local i = 1
		while i <= len do
			if not func(i, tbl[i]) then
				tremove(tbl, i)
			else
				i = i + 1
			end
		end
		return
	end
	for k, v in next, tbl do
		if not func(k, v) then
			tbl[k] = nil
		end
	end
end

local function find(tbl, func)
	if isSequential(tbl) then
		for i = 1, #tbl do
			if func(i, tbl[i]) then
				return i, tbl[i]
			end
		end
		return
	end
	for k, v in next, tbl do
		if func(k, v) then
			return k, v
		end
	end
end

local function indexOf(tbl, val)
	return find(tbl, function(k, v)
		if v == val then
			return k
		end
	end)
end

local function keys(tbl)
	local ret = {}
	for k in next, tbl do
		tinsert(ret, k)
	end
	return ret
end

local function values(tbl)
	local ret = {}
	for k, v in next, tbl do
		tinsert(ret, v)
	end
	return ret
end

local function map(tbl, func)
	for k, v in next, tbl do
		local val = func(k, v)
		if val then
			tbl[k] = v
		end
	end
end

local function reduce(tbl, func, init)
	local curr = init
	if isSequential(tbl) then
		for i = 1, #tbl do
			curr = func(i, tbl[i], curr)
		end
		return curr
	end
	for k, v in next, tbl do
		curr = func(k, v, curr)
	end
end

local function reduceRight(tbl, func, init)
	if not isSequential(tbl) then
		error("Table is not sequential")
	end
	for i = #tbl, 1, -1 do
		curr = func(i, tbl[i], curr)
	end
end

local function reverse(tbl)
	if not isSequential(tbl) then
		error("Table is not sequential")
	end
	local len = #tbl
	for i = 1, len/2 do
		tbl[i] = tbl[len - i + 1]
	end
end

local function shift(tbl)
	return tremove(tbl, 1)
end

local function slice(tbl, startIndex, endIndex)
	assertSequential(tbl)
	local len = #tbl
	
	startIndex = startIndex or 1
	endIndex = endIndex or len
	
	if startIndex < 0 then
		startIndex = len - startIndex
	end
	
	if endIndex < 0 then
		endIndex = len - endIndex
	end
	
	local ret = {}
	for i = startIndex or 1, endIndex or len do
		tinsert(ret, tbl[i])
	end
end

local function any(tbl, func)
	local k, v = find(tbl, func)
	if k then
		return true, k, v
	end
	return false
end

local function test(tbl, func)
	local k, v = find(tbl, function(k, v)
		if not func(k, v) then
			return k, v
		end
	end)
	if k then
		return false, k, v
	end
	return true
end

local function unshift(tbl)
	tinsert(tbl, 1)
end

local function removeKeysByValue(tbl, key)
	if isSequential(tbl) then
		local len = #tbl
		local i = 1
		while i <= len do
			if tbl[i] == key then
				tremove(tbl, key)
			else
				i = i + 1
			end
		end
		return
	end
	for k, v in next, tbl do
		if k == key then
			tbl[k] = nil
		end
	end
end

local function count(tbl, func)
	local c = 0
	for k, v in next, tbl do
		if not func or func(k, v) then
			c = c + 1
		end
	end
	return c
end

local function copy(dest, source)
	if not source then
		source = dest
		dest = {}
	end
	for k, v in next, source do
		dest[k] = v
	end
	return dest
end

local function copyNoOverride(dest, source)
	for k, v in next, source do
		if not dest[k] then
			dest[k] = v
		end
	end
end

local function isEmpty(tbl)
	return not next(tbl)
end

local weakKeyMT = {__mode = "k"}
local weakValueMT = {__mode = "v"}
local weakKeyValueMT = {__mode = "kv"}

local function weakKeyTable()
	return setmetatable({}, weakKeyMT)
end

local function weakValueTable()
	return setmetatable({}, weakValueMT)
end

local function weakKeyValueTable()
	return setmetatable({}, weakKeyValueMT)
end

local function swap(tbl)
	local ret = {}
	for k, v in next, tbl do
		ret[v] = k
	end
	return ret
end

local function sorted(tbl, descending, sortByValue, secondarySort)
	local data = sortByValue and values(tbl) or keys(tbl)
	local lookupMap = sortByValue and swap(tbl) or tbl
	if secondarySort then
		table.sort(data, descending and function(a, b) return lookupMap[a] > lookupMap[b] end or function(a, b) return lookupMap[a] < lookupMap[b] end)
	end
	table.sort(data, descending and function(a, b) return a > b end or nil)

	local i = 1
	if sortByValue then
		return function()
			local v = data[i]
			i = i + 1
			return lookupMap[v], v
		end
	else
		return function()
			local k = data[i]
			i = i + 1
			return k, lookupMap[k]
		end
	end
end

local function compare(tbl1, tbl2)
	for k, v in next, tbl1 do
		if v ~= tbl2[k] then
			return false
		end
	end
	for k, v in next, tbl2 do
		if v ~= tbl1[k] then
			return false
		end
	end
	return true
end

local function diff(tbl1, tbl2)
	local added, missing, changed = {}, {}, {}
	for k, v in next, tbl1 do
		if not tbl2[k] then
			missing[k] = v
		end
		if v ~= tbl2[k] then
			changed[k] = v
		end
	end
	for k, v in next, tbl2 do
		if not tbl1[k] then
			added[k] = v
		end
	end
	return added, missing, changed
end

local function traverse(tbl, func, done, parent)
	done = done or {}
	if done[tbl] then
		return
	end
	done[tbl] = true
	for k, v in next, tbl do
		if type(v) == "table" then
			traverse(v, done, func, tbl)
		else
			func(k, v, tbl, parent)
		end
	end
end

local function flatten(tbl, done, result)
	local result = {}
	traverse(tbl, function(k, v)
		result[k] = v
	end)
end

local function flattenValues(tbl, done, result)
	local result = {}
	traverse(tbl, function(k, v)
		tinsert(result, v)
	end)
end

local function min(tbl, func)
	local minV = math.huge
	local minK = nil
	for k, v in next, tbl do
		if (func and func(minV, v)) or v < minV then
			minV = v
			minK = k
		end
	end
	return minV, minK
end

local function max(tbl, func)
	local maxV = math.huge
	local maxK = nil
	for k, v in next, tbl do
		if (func and func(v, maxV)) or v > maxV then
			maxV = v
			maxK = k
		end
	end
	return minV, maxK
end

local function shuffle(tbl)
	local size = #tbl
	for i = 1, size do
		local j, k = math.random(size), math.random(size)
		tbl[j], tbl[k] = tbl[k], tbl[j]
	end
	return tbl
end

local function getRandom(tbl)
	return tbl[math.random(1, #tbl)]
end

local function getRandomAssoc(tbl)
	local rand = math.random(1, count(tbl))
	local i = 1
	for _, v in next, tbl do
		if i == rand then
			return v
		end
		i = i + 1
	end
end

local function random(size, min, max, useReal)
	local rand = useReal and mathe.randomReal or math.random
	if not min and not max then
		min = 0
		max = 1
	end
	if min and not max then
		min, max = 0, min
	end
	
	local ret = {}
	for i = 1, size do
		ret[i] = rand(min, max)
	end
	return ret
end

local function split(tbl, size)
	local ret = {}
	for i = 1, #tbl, size do
		local sub = {}
		for k = i, i + size - 1 do
			sub[k - i + 1] = tbl[k]
		end
		tinsert(ret, sub)
	end
	return ret
end

local function join(tables)
	local ret = {}
	local j = 1
	for i = 1, #tables do
		for k = 1, #tables[i] do
			ret[j] = tables[i][k]
			j = j + 1
		end
	end
	return ret
end

return {
	forEach = forEach,
	test = test,
	fill = fill,
	isSequential = isSequential,
	filter = filter,
	find = find,
	indexOf = indexOf,
	keys = keys,
	values = values,
	map = map,
	reduce = reduce,
	reduceRight = reduceRight,
	reverse = reverse,
	shift = shift,
	slice = slice,
	any = any,
	unshift = unshift,
	removeKeysByValue = removeKeysByValue,
	count = count,
	copy = copy,
	copyNoOverride = copyNoOverride,
	isEmpty = isEmpty,
	weakKeyTable = weakKeyTable,
	weakValueTable = weakValueTable,
	weakKeyValueTable = weakKeyValueTable,
	swap = swap,
	sorted = sorted,
	compare = compare,
	diff = diff,
	traverse = traverse,
	flatten = flatten,
	flattenValues = flattenValues,
	min = min,
	max = max,
	shuffle = shuffle,
	getRandom = getRandom,
	getRandomAssoc = getRandomAssoc,
	random = random,
	split = split,
	join = join
}