local shader = require 'SDL/shader'
local quad = require 'shapes/quad_gui'

local matrixModule = require 'utils/matrix'
local matrix = matrixModule.new
local matrixBase = matrixModule.new()

local events = require 'gui/events'

-- useful shortcuts for matrix transformations
local R,T,S = matrixBase.rotate, matrixBase.translate, matrixBase.scale

local M = {}
local shapes = {}
local guiShader

local focusedWindow = false
local draggingWindow = false
local windowOverCursor = false

local windowSpaceMatrix

local windowMovementStatus = {
	startX = 0,
	startY = 0,
}

local topWindow
local guiInterface = {}

local function prepareGUIshapes()
	shapes.quad = quad.new()
end

local function prepareGUIshader(appWindow)
	guiShader = shader.load('../../common/shaders/gui')

	guiShader.use()
	local uniforms = guiShader.uniform

	guiInterface.shader = guiShader
	guiInterface.shapes = shapes

	uniforms.define {
		{name = 'baseColor', type = {'f', 4}},
		{name = 'diffuseTexture', type = {'i'}},
		{name = 'modelViewMatrix', type = {'f', 4, 4, true}},
		{name = 'projectionMatrix', type = {'f', 4, 4, true}},
		{name = 'texMatrix', type = {'f', 3, 3, true}},
		{name = 'textured', type = {'i'}},
	}

	local maxDepth = 16378

	--[[
	You need to convert viewport coordinates into OpenGL default screen-space.
	It's done by using inverted transformation matrix where you actually reverse whole process. Therefore, matrix transformations need to be done in reverse order.

	-1, 1                 1, 1     -1, -1                 1, -1      0, 0                 2, 0       0, 0                 1, 0       0, 0                 width, 0
		+-----------------+				+-----------------+				+-----------------+				+-----------------+				+-----------------+
		|                 |             |                 |         	|                 |             |                 |             |                 |
		|        1.       |    --->     |        2.       |    ---> 	|        3.       |     --->    |        4.       |    --->     |        5.       |
		|                 |             |                 |         	|                 |             |                 |             |                 |
		+-----------------+				+-----------------+				+-----------------+				+-----------------+				+-----------------+
	-1, -1                1, -1     -1, 1                 1, 1       0, 2                 2, 2       0, 1                 1, 1       0, height            width, height



	--]]

	windowSpaceMatrix = (S(appWindow.width, appWindow.height, maxDepth) * S(1/2, 1/2, 1) * T(1, 1, 1) * S(1, -1, -1) ).inv()

	uniforms {
		baseColor = {1,1,1,1},
		diffuseTexture = 0,
		modelViewMatrix = matrix(),
		projectionMatrix = windowSpaceMatrix,
		texMatrix = {
			1,0,0,
			0,1,0,
			0,0,1,
		},
	}

	-- bind vertex attributes
	guiShader.bind {
		[0] = 'VertexPosition',
		[1] = 'VertexTexCoord',
	}
end

local function prepareGUIhandlers()
	events.add('mouseButtonDown', function(x, y, button)
		if topWindow then
			topWindow.propagateSignal('mouseButtonDown', x, y, button)
		end
	end)

	events.add('mouseMove', function(x, y)
		if draggingWindow then
		    local projectedX, projectedY = draggingWindow.projectCursorToWindow(x, y)

		    local movementMatrix = T(
				projectedX - windowMovementStatus.startX,
				projectedY - windowMovementStatus.startY,
				0
			)

			local newWindowPosition = movementMatrix * {draggingWindow.properties.x, draggingWindow.properties.y, draggingWindow.properties.z, 1}
			draggingWindow.properties.x = newWindowPosition[1]
			draggingWindow.properties.y = newWindowPosition[2]
			draggingWindow.properties.z = newWindowPosition[3]
		end

		if topWindow then
			-- call mouseButtonUp signal
			topWindow.propagateSignal('mouseMove', x, y)
		end
		return true
	end)

	events.add('keyDown', function(keySym, keyMod)
		if focusedWindow then
			-- call keyDown signal
			focusedWindow.callSignal('keyDown', keySym, keyMod)
		end
		return true
	end)

	events.add('keyUp', function(keySym, keyMod)
		if focusedWindow then
			-- call keyDown signal
			focusedWindow.callSignal('keyUp', keySym, keyMod)
		end
		return true
	end)

	events.add('mouseButtonUp', function(x, y, button)
		if button==1 and draggingWindow then
			draggingWindow = false
		end
		
		if topWindow then
			-- call mouseButtonUp signal
			topWindow.propagateSignal('mouseButtonUp', x, y, button)
		end
	end)
end

local function setupDraggableElement(element)
	-- add default signals for window dragging
	element.addSignal('mouseButtonDown',
		function(self, x, y, button)
			if self.isMouseOverWindow(x, y) and self.properties.draggable then
				-- left mouse button uses number 1 identifier
				if button==1 and (draggingWindow ~= self) then
				    local projectedX, projectedY = self.projectCursorToWindow(x, y)

					windowMovementStatus.startX = projectedX
					windowMovementStatus.startY = projectedY
					draggingWindow = self
				end
			end
			return true
		end
	)
end

local function setupElement(element)
	element.hasFocus = function()
		return focusedWindow == element
	end

	-- handle window focus state
	element.addSignal('mouseButtonUp',
		function(self, x, y, button)
			if self.isMouseOverWindow(x, y) then
				if button==1 and not self.hasFocus() then
					if focusedWindow then
						focusedWindow.callSignal('lostFocus')
		    	    end
        			focusedWindow = self
					focusedWindow.callSignal('focus')
				end
			end
			return true
		end
	)

	element.addSignal('mouseMove', function(self, x, y)
	    local projectedX, projectedY = self.projectCursorToWindow(x, y)
	end)

	setupDraggableElement(element)
   
	for _, child in ipairs(element.children) do
   		child.parent = element
   	end
end

local function createElement(name, def, ...)
	assert(type(name)=='string', 'You have to specify element name')

	local status, prototype = pcall(function()
		local path = ('gui/elements/%s'):format(name)
		return require(path)
	end)

	if status then
		local element = prototype.new(guiInterface, def, ...)
		setupElement(element)
		return element
	else
		error(("Unknown element name (%s)"):format(name))
	end
end

local function prepare(appWindow, font, abstractWindowName)
	local abstractWindowName = abstractWindowName or 'abstract'
	prepareGUIshapes()
	prepareGUIshader(appWindow)
	prepareGUIhandlers()

	-- abstract window proxy - good for other window types that inherit a base from the abstract window
	guiInterface.abstract = function(properties)
		return createElement(abstractWindowName, properties)
	end

	font.setupProjectionMatrix(windowSpaceMatrix)
	guiInterface.print = font.printM

	topWindow = createElement(abstractWindowName, {
		properties = {
			x = 0,
			y = 0,
			originX = 0,
			originY = 0,
			width = appWindow.width,
			height = appWindow.height,
			color = {1,1,1,1},
			name = 'App window',
			draggable = false,
			hidden = true,
			clip = false,
		},
	})

	M.topWindow = topWindow
end

local function addElement(element)
	table.insert(topWindow.children, element)
	element.parent = topWindow
end

local function update()
	if topWindow then
		topWindow.update()
	end
end

local function draw()
	if topWindow then
		topWindow.draw()
	end
end

M.prepare = prepare
M.invoke = events.invoke
M.create = createElement
M.add = addElement
M.update = update
M.draw = draw

return M
