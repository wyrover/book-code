local M = {}

local _min, _max = math.min, math.max

--[[
	local f1 = fuzzy.solver()
	local L = f1.L
	local F = f1.F
	local R = f1.R
	
	local service = F {'service', {0, 10}}
	service['poor'] = L {fuzzy.gauss, {1.5, 0}}
	service['good'] = L {fuzzy.gauss, {1.5, 5}}
	service['excelent'] = L {fuzzy.gauss, {1.5, 10}}

	local food = F {'food', {0, 10}}
	food['rancid'] = L {fuzzy.trapezoid, {0, 0, 1, 3}}
	food['delicious'] = L {fuzzy.trapezoid, {7, 9, 10, 10}}

	local tip = F {'tip', {0, 30}}
	tip['cheap'] = L {fuzzy.triangle, {0, 5, 10}}
	tip['average'] = L {fuzzy.triangle, {10, 15, 20}}
	tip['generous'] = L {fuzzy.triangle, {20, 25, 30}}

	local r1 = R(1)
	r1.premise = service['poor'] + food['rancid']
	r1.implication = tip['cheap']

	local r2 = R(1)
	r2.premise = service['good']
	r2.implication = tip['average']

	local r3 = R(1)
	r3.premise = service['excelent'] + food['delicious']
	r3.implication = tip['generous']

	local values = {
		service = 8.0,
		food = 6.5,
	}
	-- @22.2115
	local result = f1(values)
	print(result)

--]]

M.gauss = function(deviation, mean)
	return function(_x)
		return function()
			local x = _x()
			return math.exp( -((x - mean)^2)/(2*deviation^2))
		end
	end
end

M.trapezoid = function(a, b, c, d)
	return function(_x)
		return function()
			local x = _x()
			if x>=a and x<b then
				return (x - a)/(b-a)
			elseif x>=b and x<c then
				return 1
			elseif x>=c and x<d then
				return (d - x)/(d-c)
			else
				return 0
			end
		end
	end
end

M.triangle = function(a, b, c)
	return function(_x)
		return function()
			local x = _x()
			if x>=a and x<b then
				return (x - a)/(b-a)
			elseif x>=b and x<c then
				return (c - x)/(c-b)
			else
				return 0
			end
		end
	end
end

M.min = function(a, b)
	return function(x, y)
		return _min(a(x), b(y))
	end
end

M.max = function(a, b)
	return function(x, y)
		return _max(a(x), b(y))
	end
end

M.neg = function(a)
	return function(x)
		return 1.0 - a(x)
	end
end

M.crop = function(rangeMin, rangeMax)
	return function(x)
		return _min(_max(x, rangeMin), rangeMax)
	end
end

local function mergeSets(a, b)
	local out = {}
	for k, _ in pairs(a) do
		out[k] = true
	end
	if type(b)=='table' then
		for k, _ in pairs(b) do
			out[k] = true
		end
	end
	return out
end

M.solver = function()
	local rules = {}
	local s_out = {
		rules = rules,
		settings = {
			implicationOperator = _min,
			aggregationOperator = _max,
			step = 0.01,
		},
	}

	local fuzzySets = {
	}

	local outputFuzzySets = {
	}

	-- linguistic variable
	s_out.L = function(def)
		assert(type(def)=='table')
		local fnGen, parameters = unpack(def)
		assert(type(fnGen)=='function' and type(parameters)=='table')
		return fnGen(unpack(parameters))
	end

	-- fuzzy set
	s_out.F = function(def)
		assert(type(def)=='table')
		local name, range = unpack(def)
		assert(type(name)=='string' and type(range)=='table')

		local out = {}
		local range = {_min(unpack(range)), _max(unpack(range))}
		out.range = range
		out.name = name
		local langValues = {}; out.langValues = langValues
		fuzzySets[name] = {
			value = 0,
			range = range,
		}
		
		local crop = M.crop(unpack(range))
		local cropValue = M.crop(0, 1)
		local min, max, neg = M.min, M.max, M.neg

		local function getFuzzyValueFunctor()
			return function()
				return crop(fuzzySets[name].value)
			end
		end

		local function prepareFuzzySet(fn)
			local var = {
				fn = fn,
				crop = crop,
				fuzzySets = {
					[out] = true,
				},
			}

			setmetatable(var, {
				-- OR
				__add = function(a, b)
					local fs = prepareFuzzySet(max(a.fn, b.fn))
					fs.fuzzySets = mergeSets(a.fuzzySets, b.fuzzySets) 
					return fs
				end,
				-- AND
				__mul = function(a, b)
					local fs = prepareFuzzySet(min(a.fn, b.fn))
					fs.fuzzySets = mergeSets(a.fuzzySets, b.fuzzySets) 
					return fs
				end,
				-- NOT
				__unm = function(a)
					local fs = prepareFuzzySet(neg(a.fn))
					fs.fuzzySets = mergeSets(a.fuzzySets)
					return fs
				end,
				__call = function(a)
					return cropValue(fn())
				end,
			})
			return var
		end

		setmetatable(out, {
			__index = function(_, name)
				return langValues[name]
			end,
			__newindex = function(_, name, fuzzySetFn)
				assert(type(fuzzySetFn)=='function')
				langValues[name] = prepareFuzzySet(
					-- prepare elementary fuzzy set
					fuzzySetFn(getFuzzyValueFunctor())
				)
			end,
		})
		return out
	end

	-- rule
	s_out.R = function(factor)
		local out = {
			premise = false,
			implication = false,
			factor = factor or 1,
		}
		table.insert(rules, out)
		return out
	end

	local function computeImplication(rule)
		local premiseResult = rule.premise()
		local implication = rule.implication
		local factor = rule.factor or 1

		local out = {
		}

		local implicationFuzzySets = implication.fuzzySets
		local langValues = implication.langValues
		local implicationOperator = s_out.settings.implicationOperator
		local oldValues = {}
		local range = {0, 0}

		-- store current values and obtain maximum range
		for fs, _ in pairs(implicationFuzzySets) do
			oldValues[fs.name] = fuzzySets[fs.name].value
			range[1] = _min(range[1], fs.range[1])
			range[2] = _max(range[2], fs.range[2])
		end

		-- compute values for each implication conclusion
		for x = range[1], range[2], s_out.settings.step do
			for fs, _ in pairs(implicationFuzzySets) do
				fuzzySets[fs.name].value = x
			end
			local result = implicationOperator(implication(), premiseResult)
			table.insert(out, {x, result*factor})
		end

		-- restore old value
		for fs, _ in pairs(implicationFuzzySets) do
			fuzzySets[fs.name].value = oldValues[fs.name]
		end
		return out
	end

	local function aggregateResults(ruleResults)
		local aggregatedResult = {}
		local ruleResult1 = ruleResults[1]
		local aggregationOperator = s_out.settings.aggregationOperator

		if #ruleResults > 1 then
			for i, point in ipairs(ruleResult1) do
				local values = {point[2]}
				for j=2,#ruleResults do
					table.insert(values, ruleResults[j][i][2])
				end
				
				aggregatedResult[i] = {point[1], aggregationOperator(unpack(values))}
			end
		else
			aggregatedResult = ruleResults[1]
		end

		return aggregatedResult
	end

	-- centroid
	local function defuzzifyResult(fuzzyResult)
		local accumXY, accumY = 0, 0
		for i, point in ipairs(fuzzyResult) do
			if point[2] > 0 then
				accumXY = accumXY + point[1]*point[2]
				accumY = accumY + point[2]
			end
		end
		
		if accumY > 0 then  
			return accumXY/accumY
		else
			return (fuzzyResult[1][2]-fuzzyResult[#fs][2])/2
		end
	end

	local solve = function(_, inputs)
		for name, value in pairs(inputs) do
			fuzzySets[name].value = value
		end
		--outputFuzzySets
		local partialResults = {}
		for i, rule in ipairs(rules) do
			local implicationFSs = rule.implication.fuzzySets
			for FS, _ in pairs(implicationFSs) do
				local result = partialResults[FS.name]
				if not result then
					result = {}; partialResults[FS.name] = result
				end
				table.insert(result, computeImplication(rule))
			end
		end
		for name, results in pairs(partialResults) do
			local aggResult = aggregateResults(results)
			outputFuzzySets[name] = defuzzifyResult(aggResult)
		end

		return outputFuzzySets
	end

	setmetatable(s_out, {
		__call = solve,
	})

	return s_out
end


return M
