local matrixModule = require 'utils/matrix'
local matrix = matrixModule.new
local matrixBase = matrixModule.new()

-- useful shortcuts for matrix transformations
local R,T,S = matrixBase.rotate, matrixBase.translate, matrixBase.scale

local defaultProperties = {
	textColor = {r=0,g=0,b=0,a=1},
}

local function createElement(interface, def)
	local obj = interface.abstract(def)

	local properties = obj.properties or {}
	local children = obj.children or {}

	local shader = interface.shader
	local shapes = interface.shapes
	local gprint = interface.print

	local uniforms = shader.uniform

	-- default values
	for name, default in pairs(defaultProperties) do
		if type(properties[name])=='nil' then
			properties[name] = default
		end
	end

	obj.addSignal('draw', function(self, modelviewMatrix)
		local fontMatrix0 = modelviewMatrix * T(-64, -properties.height/2 + 32, 1) * S(0.2, 0.2, 1) 

		gprint(fontMatrix0, properties.textColor, properties.name)
	end)

	return obj
end

return {
	new = createElement,
}