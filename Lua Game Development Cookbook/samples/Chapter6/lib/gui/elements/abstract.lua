local matrixModule = require 'utils/matrix'
local matrix = matrixModule.new
local matrixBase = matrixModule.new()

-- useful shortcuts for matrix transformations
local R,T,S = matrixBase.rotate, matrixBase.translate, matrixBase.scale

local defaultProperties = {
	x = 0,
	y = 0,
	z = 0,
	relativeX = 0,
	relativeY = 0,
	relativeZ = 0,
	rotateX = 0,
	rotateY = 0,
	rotateZ = 0,
	angle = 0,
	width = 0,
	height = 0,
	originX = 0.5,
	originY = 0.5,
	originZ = 0,
	color = {1,1,1,1},
	visible = true,
	enabled = true,
	hidden = false,
	draggable = true,
}

local function createElement(interface, def)
	local properties = def.properties or {}
	local children = def.children or {}
	local signals = {}

	local shader = interface.shader
	local shapes = interface.shapes

	local obj = {
		properties = properties,
		children = children,
		signals = signals,
	}

	local uniforms = shader.uniform
	local modelViewMatrix = matrix()
	local tempMatrix

	-- default values
	for name, default in pairs(defaultProperties) do
		if type(properties[name])=='nil' then
			properties[name] = default
		end
	end

	local onWindowEvents = {
    	'mouseMove',
	    'mouseButtonDown',
	    'mouseButtonUp',
	}

	obj.propagateSignal = function(name, ...)
		if properties.enabled then
			local propagate, callSignal = true, true
			for _, eventName in ipairs(onWindowEvents) do
				if eventName == name then
					local mouse_x, mouse_y = unpack {...}
					if obj.isMouseOverWindow(mouse_x, mouse_y) then
						propagate = false
					else
						callSignal = false
					end
					break
				end
			end
		
			for _, child in ipairs(children) do
				if not child.propagateSignal(name, ...) then
					return false
				end
			end

			if callSignal then
				obj.callSignal(name, ...)
			end
			return propagate
		else
			return false
		end
	end

	obj.callSignal = function(name, ...)
		local list = signals[name]
		if type(list)=="table" then
			for i, action in ipairs(list) do
				if type(action)=="function" then
					if not action(obj, ...) then
						return false
					end
				end
			end
		end
		return true
	end

	obj.addSignal = function(name, fn)
		if not signals[name] then
			signals[name] = {}
		end
		local list = signals[name]
		if type(list)=="table" and type(fn)=="function" then
			table.insert(list, fn)
		end
	end

	obj.update = function(...)
		local outerModelViewMatrix = {...}
		modelViewMatrix = matrix()

		local scaleMatrix = S(properties.width, properties.height, 1)
		local invScaleMatrix = scaleMatrix.inv()

		-- this matrix will be useful in projectCursorToWindow function
		tempMatrix =
			scaleMatrix
			* T(properties.originX, properties.originY, properties.originZ)

		table.insert(outerModelViewMatrix,
			T(properties.x, properties.y, properties.z)
			* R(properties.rotateX, properties.rotateY, properties.rotateZ, properties.angle)
			* tempMatrix
		)

		for _, m in ipairs(outerModelViewMatrix) do
			modelViewMatrix = modelViewMatrix * m
    	end

		for _, child in ipairs(children) do
			local properties = child.properties

			child.update(
				modelViewMatrix,
				T(properties.relativeX, properties.relativeY, properties.relativeZ),
				invScaleMatrix
			)
		end
	end

	obj.draw = function()
		if properties.visible then
			if not properties.hidden then
				shader.use()
				uniforms {
					modelViewMatrix = modelViewMatrix,
					baseColor = properties.color,
					textured = (properties.textured and 1) or 0,
				}
				shapes.quad.draw()
			end

			-- draw children windows
			for _, child in ipairs(children) do
				child.draw()
			end
		end
	end

	obj.relativeCoordinates = function(x, y)
		return modelViewMatrix.inv() * {x, y, 0, 1}
	end

	obj.projectCursorToWindow = function(x, y)
		local relativeMouseCoords = obj.relativeCoordinates(x, y)

		local mouseCoordsOnWindow =
			tempMatrix
			* relativeMouseCoords

		return mouseCoordsOnWindow[1], mouseCoordsOnWindow[2]
	end

	obj.isMouseOverWindow = function(x, y)
		local relativeMouseCoords = obj.relativeCoordinates(x, y)
		local wX, wY = relativeMouseCoords[1], relativeMouseCoords[2]
		return (wX<=0.5 and wX>=-0.5 and wY<=0.5 and wY>=-0.5)
	end

	obj.update()

	return obj
end

return {
	new = createElement,
}