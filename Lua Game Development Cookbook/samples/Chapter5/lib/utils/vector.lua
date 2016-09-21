local baseNames = {'x','y','z','w'}
local baseNamesRev = {['x'] = 1, ['y'] = 2, ['z'] = 3, ['w'] = 4}
local sin = math.sin
local cos = math.cos
local sqrt = math.sqrt

local function det(v, rows, cols)
	assert(rows == cols, 'This must be square matrix')
	local sum = 0
	for i=0,cols-1 do
		local subSum = 1
		for j=0,cols-1 do
			local pos = (j+i)%cols + 1
			subSum = subSum * v[pos][pos]
		end
		sum = sum + subSum
	end
	for i=0,cols-1 do
		local subSum = 1
		for j=0,cols-1 do
			local pos = (j+i)%cols + 1
			subSum = subSum * v[pos][cols-(pos-1)]
		end
		sum = sum - subSum
	end
	return sum
end

local function vector(n, v)
	local n = n or 1
	local v = v or (function() local o = {} for i=1,n do o[i]=0 end return o end)()
	
	local fn = {
		zeros = function()
			for i=1,n do
				v[i] = 0
			end
		end,
		ones = function()
			for i=1,n do
				v[i] = 1
			end
		end,
		resize = function(n)
			local out = {}
			for i=1,n do
				out[i] = rawget(v,i) or 0
			end
			return vector(n, out)
		end,
		dup = function()
			local out = {}
			for i=1,n do
				out[i] = v[i]
			end
			return vector(n, out)
		end,
		normalize = function()
			local out = {}
			local len
			do
				local sum = 0
				for i=1,n do
					sum = sum + v[i]*v[i]
				end
				len = sqrt(sum)
			end
			if len == 0 then
				for i=1,n do
					out[i] = v[i]
				end
			else
				for i=1,n do
					out[i] = v[i]/len
				end
			end
			return vector(n, out)
		end,
		dot = function(b)
			local sum = 0
			for i=1,n do
				sum = sum + v[i]*b[i]
			end
			return sum
		end,
		cross = function(b)
			local out = {}
			if n==3 then
				out[1] = det({
					{v[2],v[3]},
					{b[2],b[3]},
				}, 2, 2)
				out[2] = -det({
					{v[1],v[3]},
					{b[1],b[3]},
				}, 2, 2)
				out[3] = det({
					{v[1],v[2]},
					{b[1],b[2]},
				}, 2, 2)
			end
			return vector(n, out)
		end,
	}

	local mt = {
		__newindex = function(t, i , value)
			if type(i)=="string" then
				local basePos = baseNamesRev[i]
				if type(basePos)=="number" then
					local base = rawget(v,basePos)
					assert(type(base)=="number", ('Invalid base name: %q'):format(tostring(i)))
					rawset(v, basePos, value)
				end
			elseif type(i)=="number" then
				local base = rawget(v,i)
				assert(type(base)=="number", ('Invalid base name: %q'):format(tostring(i)))
				rawset(v, i, value)
			end
		end,
		__index = function(t, i)
			if type(i)=="string" then
				local basePos = baseNamesRev[i]
				if type(basePos)=="number" then
					local base = rawget(v,basePos)
					assert(type(base)=="number", ('Invalid base name: %q'):format(tostring(i)))
					return base
				else
					local f = fn[i]
					if type(f)=='function' then
						return f
					end
				end
			elseif type(i)=="number" then
				local base = rawget(v,i)
				assert(type(base)=="number", ('Invalid base name: %q'):format(tostring(i)))
				return base
			else
				return rawget(m, i)
			end
		end,
		__add = function(a, b)
			local out = {}
			if type(b)=="number" then
				for i=1,n do
					out[i] = a[i] + b
				end
			else
				for i=1,n do
					out[i] = a[i] + b[i]
				end
			end
			return vector(n, out)
		end,
		__sub = function(a, b)
			local out = {}
			if type(b)=="number" then
				for i=1,n do
					out[i] = a[i] - b
				end
			else
				for i=1,n do
					out[i] = a[i] - b[i]
				end
	        end
			return vector(n, out)
		end,
		__unm = function(a)
			local out = {}
			for i=1,n do
				out[i] = -a[i]
			end
			return vector(n, out)
		end,
		__mul = function(a, b)
			local out = {}
			if type(b)=="number" then
				for i=1,n do
					out[i] = a[i]*b
				end
			else
				for i=1,n do
					out[i] = a[i]*b[i]
				end
		    end
			return vector(n, out)
		end,
		__pow = function(a, b)
			local out = {}
			if type(b)=="number" then
				for i=1,n do
					out[i] = a[i]^b
				end
			else
				for i=1,n do
					out[i] = a[i]^b[i]
				end
		    end
			return vector(n, out)
		end,
		__div = function(a, b)
			local out = {}
			if type(b)=="number" then
				for i=1,n do
					out[i] = a[i]/b
				end
			else
				for i=1,n do
					out[i] = a[i]/b[i]
				end
		    end
			return vector(n, out)
		end,
		__len = function(a)
			local sum = 0
			for i=1,n do
				sum = sum + v[i]*v[i]
			end
			return sqrt(sum)
		end,
		__tostring = function (a)
			local out = {
				('Vector(%d) {'):format(n),
			}
			for i=1,n-1 do
				table.insert(out, ("%04f, "):format(tonumber(v[i])))
			end
			table.insert(out, ("%04f}"):format(tonumber(v[n])))
			return table.concat(out)
		end,
	}

	setmetatable(v, mt)
	return v
end

return {
	new = vector,
}