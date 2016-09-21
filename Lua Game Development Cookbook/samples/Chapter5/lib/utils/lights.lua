local matrixModule = require 'utils/matrix'
local matrix = matrixModule.new
local matrixBase = matrixModule.new()

local box = require 'shapes/box'

local lightDefaults = {
	position = {0,0,0,1},
	diffuse = {1,0.8,0.8,1},
	specular = {1,1,1,1},
	spotCutoff = 100,
	spotExponent = 1.2,
	constantAttenuation = 0,
	linearAttenuation = 1,
	quadraticAttenuation = 0,
	spotDirection = {0,0,0},
}

local function initLights(shader, lightDefs)
    local uniform = shader.uniform
    local lights = {}
    local boxShape = box.new()

	local function defineLight(index)
		assert(type(index)=='number' and index>=0)
		local prefix = ('lights[%d].'):format(index)

		uniform.define {
			{name = prefix..'position', type = {'f', 4}},
			{name = prefix..'diffuse', type = {'f', 4}},
			{name = prefix..'specular', type = {'f', 4}},
			{name = prefix..'spotCutoff', type = {'f'}},
			{name = prefix..'spotExponent', type = {'f'}},
			{name = prefix..'constantAttenuation', type = {'f'}},
			{name = prefix..'linearAttenuation', type = {'f'}},
			{name = prefix..'quadraticAttenuation', type = {'f'}},
			{name = prefix..'spotDirection', type = {'f', 3}},
		}
	end

	local function setLight(index, lightDefinition)
		local prefix = ('lights[%d].'):format(index)

		local paramTable = {}
		for k,v in pairs(lightDefaults) do
			local value = lightDefinition[k] or v
		    lightDefinition[k] = value
			paramTable[prefix..k] = value
		end

		uniform(paramTable)

		local lightObject = {
			matrix = matrix(),
		}

		setmetatable(lightObject, {
			-- obtain light parameter
			__index = function(t,k)
				return lightDefinition[k]
			end,
			-- update light parameter
			__newindex = function(t, k, v)
			    uniform[prefix..k] = v
				lightDefinition[k] = v
			end,
			-- draw a light box
			__call = function(t)
				shader.use()

				uniform['object.view'] = t.matrix * matrixBase.scale(0.1, 0.1, 0.1)
				uniform['debug'] = 1
				uniform['debugColor'] = lightDefinition.diffuse

				boxShape.draw(shader)
				uniform['debug'] = 0
			end,
		})
		return lightObject
	end

	-- define light parameters types
	for i=0,#lightDefs-1 do
		defineLight(i)
	end
	for i,lightParameters in ipairs(lightDefs) do
		lights[i] = setLight(i-1, lightParameters)
	end

	uniform['totalLights'] = #lightDefs

	return lights
end

return {
	init = initLights,
}