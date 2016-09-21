local matrixModule = require 'utils/matrix'
local matrix = matrixModule.new
local matrixBase = matrixModule.new()

local keyboardBinding = {
	forward = SDL.SDLK_w,
	backward = SDL.SDLK_s,
	strafeLeft = SDL.SDLK_a,
	strafeRight = SDL.SDLK_d,
	flyUp = SDL.SDLK_q,
	flyDown = SDL.SDLK_z,
}

local function projectionMatrix(fov, aspect, znear, zfar)
	-- xymax variable refers to the coordinate
	-- of the right/bottom clip-plane
	local xymax = znear * math.tan(math.rad(fov/2))
	local ymin = -xymax -- top clip-plane
	local xmin = -xymax -- left clip-plane

	local width = xymax - xmin
	local height = xymax - ymin
	local depth = zfar - znear

	-- q and qn parameters are used to achieve
	-- perspective correction
	local q = -(zfar + znear) / depth
	local qn = -2 * (zfar * znear) / depth

	local w = 2 * znear / width
	w = w / aspect
	local h = 2 * znear / height

	-- transposed version of the projection matrix
	return {
		w, 0, 0, 0,
		0, h, 0, 0,
		0, 0, q, -1,
		0, 0, qn, 0,
	}
end

local function initCamera(screenAspectRatio, keyStates, mouseState, shader)
	local movementSpeed = 0.05
	local uniform = shader.uniform

	local properties = {
		orientationVector = {0,0,1,0},
		position = {0, 0, 4, 1},
	}

	local cameraMatrix = matrix()
	local deltaRotationMatrix = matrix()
	local finalCameraMatrix

	local function update(dt, orientationChanged)
		shader.use()
		local translateVector = false

		-- move forward
		if keyStates[keyboardBinding.forward] and not keyStates[keyboardBinding.backward] then
			translateVector = {0,0,-1,0}
		end

		-- move backward
		if keyStates[keyboardBinding.backward] and not keyStates[keyboardBinding.forward] then
			translateVector = {0,0,1,0}
		end

		-- strafe left
		if keyStates[keyboardBinding.strafeLeft] and not keyStates[keyboardBinding.strafeRight] then
			translateVector = {-1,0,0,0}
		end

		-- strafe right
		if keyStates[keyboardBinding.strafeRight] and not keyStates[keyboardBinding.strafeLeft] then
			translateVector = {1,0,0,0}
		end

		-- fly up
		if keyStates[keyboardBinding.flyUp] and not keyStates[keyboardBinding.flyDown] then
			translateVector = {0,1,0,0}
		end

		-- fly down
		if keyStates[keyboardBinding.flyDown] and not keyStates[keyboardBinding.flyUp] then
			translateVector = {0,-1,0,0}
		end

		if translateVector then
			local scaleMatrix = matrixBase.scale(movementSpeed, movementSpeed, movementSpeed)
			local deltaTranslateMatrix = scaleMatrix * matrixBase.translate(unpack(translateVector)) * scaleMatrix.inv()

			cameraMatrix = cameraMatrix * deltaRotationMatrix * deltaTranslateMatrix
			
			properties.position = cameraMatrix * {0,0,0,1}
			finalCameraMatrix = cameraMatrix
		elseif orientationChanged then
			cameraMatrix = cameraMatrix * deltaRotationMatrix 
			finalCameraMatrix = cameraMatrix
		end

		properties.orientationVector = cameraMatrix * {0, 0, 1, 0}
		-- must use matrix inverse to move whole scene against camera
		uniform['camera.view'] = finalCameraMatrix
	end

	local function updateViewOrientation(dt)
		local dRx = mouseState.relativeX*mouseState.speedMultiplier.x
		local dRy = mouseState.relativeY*mouseState.speedMultiplier.y

		deltaRotationMatrix = 
			matrixBase.rotateY(math.rad(-dRx)) *
			matrixBase.rotateX(math.rad(-dRy))

		update(dt, true)
	end

	local function init()
		shader.use()

		cameraMatrix = matrixBase.rotate(0,-1,0, math.pi)  * matrixBase.translate(unpack(properties.position))
		uniform['camera.perspective'] = projectionMatrix(60, screenAspectRatio, 0.01, 100)
		uniform['camera.view'] = cameraMatrix.inv()
	end

	local M = {
		init = init,
		update = update,
		updateViewOrientation = updateViewOrientation,
	}
	setmetatable(M, {
		__index = properties,
	})

	return M
end


return {
	new = initCamera,
}