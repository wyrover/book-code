local fuzzy = require 'ai/fuzzy'

do
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

	local feel = F {'feel', {0, 10}}
	feel['bad'] = L {fuzzy.gauss, {1.5, 0}}
	feel['average'] = L {fuzzy.gauss, {1.5, 5}}
	feel['great'] = L {fuzzy.gauss, {1.5, 10}}

	local r1 = R(1)
	r1.premise = service['poor'] + food['rancid']
	r1.implication = tip['cheap']

	local r2 = R(1)
	r2.premise = service['good']
	r2.implication = tip['average']

	local r3 = R(1)
	r3.premise = service['excelent'] + food['delicious']
	r3.implication = tip['generous']

	local r4 = R(1)
	r4.premise = service['poor'] + food['rancid']
	r4.implication = feel['bad']

	local r5 = R(1)
	r5.premise = service['good'] * (-food['rancid'])
	r5.implication = feel['average']

	local r6 = R(1)
	r6.premise = service['excelent'] * food['delicious']
	r6.implication = feel['great']

	local values = {
		service = 8.0,
		food = 6.5,
	}

	-- @22.2115
	local result = f1(values)

	for k,v in pairs(result) do
		print(k, v)
	end

end