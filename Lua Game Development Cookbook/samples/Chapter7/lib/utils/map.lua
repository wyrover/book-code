local serpent = require 'serpent'
local bit = require 'bit'
local map2D = require 'map2d'

return function(def)
	local map = def or {}
	map.width = def.width or 31
	map.height = def.height or 31
	map.map = map2D(1)
	map.entry = def.entry or {x = 2, y = 2}
	map.exit = def.exit or {x = 30, y = 4}
	map.finishOnExit = def.finishOnExit or false

	local mapCells = map.map
	local sx, sy = map.entry.x, map.entry.y
	local fx, fy = map.exit.x, map.exit.y

	local shapes = {
		['c'] = {
			w = 8, h = 12,
			data = {
				0,0,0,0,0,0,1,1,
				1,1,1,1,0,0,0,1,
				1,1,1,1,1,0,0,0,
				1,1,1,1,1,1,0,0,
				1,1,1,1,1,1,0,0,
				1,1,1,1,1,1,0,0,
				1,1,1,1,1,1,0,0,
				1,1,1,1,1,1,0,0,
				1,1,1,1,1,1,0,0,
				1,1,1,1,1,0,0,0,
				0,0,1,1,0,0,0,1,
				1,0,0,0,0,0,1,1,
			}
		}
	}

	local function pushShape(name, offset, orientation)
		local shape =  shapes[name]
		local data = shape.data
		local orientation = orientation or {1, 1}

		for y=0,shape.h-1 do
			for x=0,shape.w-1 do
				local pos = {offset[1] + x, offset[2] + y}
				local sx, sy
				if orientation[1]>0 then
					sx = math.floor(x*orientation[1])
				else
					sx = math.floor(shape.w + x*orientation[1] - 1)
				end
				if orientation[2]>0 then
					sy = math.floor(y*orientation[2])
				else
					sy = math.floor(shape.h + y*orientation[2] - 1)
				end

				local v = data[sx + sy*shape.w + 1]
				if mapCells[pos] ~= 0 then
					mapCells[pos] = v
				end
			end
		end
	end

	local genMap = coroutine.wrap(function()
		pushShape('c', {12, 4}, {1,1})
		pushShape('c', {17, 8}, {-1,1})
		coroutine.yield(1)
	end)

	map.iterator = function()
		local co = coroutine.create(function()
			for y=1,map.height do
				for x=1,map.width do
					local position = {x, y}
					coroutine.yield(position, mapCells[position])
				end
			end
			coroutine.yield()
		end)
		return function()
			local code, pos, cell = coroutine.resume(co)
			return pos, cell
		end
	end

	map.load = function(data)
		local r, t =  serpent.load(data)
		if r then
			map.width = t.width
			map.height = t.height
			map.entry = t.entry
			map.exit = t.exit
			map.finishOnExit = t.finishOnExit
			map.map.load(t.map)
		end
	end

	map.save = function()
		return serpent.dump({
			map = map.map.save(),
			width = map.width,
			height = map.height,
			entry = map.entry,
			exit = map.exit,
			finishOnExit = map.finishOnExit,
		})
	end

	map.generate = genMap
	return map
end