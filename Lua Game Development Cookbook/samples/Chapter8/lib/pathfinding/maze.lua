local serpent = require 'utils/serpent'
local bit = require 'bit'
local stack = require 'utils/stack'
local map2D = require 'pathfinding/map2d'

return function(def)
	local function prepareCell(t)
		local cell = {
			type = t or 0,
			connections = 0,
		}
		return cell
	end

	local maze = def or {}
	maze.width = def.width or 31
	maze.height = def.height or 31
	maze.map = map2D(false)
	maze.entry = def.entry or {x = 2, y = 2}
	maze.exit = def.exit or {x = 30, y = 4}
	maze.finishOnExit = def.finishOnExit or false

	local map = maze.map
	local sx, sy = maze.entry.x, maze.entry.y
	local fx, fy = maze.exit.x, maze.exit.y

	local positionStack = stack()

	local neighbordLocations = {
		[0] = {0, 1},
		[1] = {1, 0},
		[2] = {0, -1},
		[3] = {-1, 0},
	}

	local function neighbours(position0, fn)
		local neighbours = {}
		if type(fn)=="function" then
			for i=0,3 do
				local neighbourLocation = neighbordLocations[i] 
				local position1 = {position0[1] + neighbourLocation[1], position0[2] + neighbourLocation[2]}
				if (position1[1]>=1 and position1[1] <= maze.width and position1[2]>=1 and position1[2] <= maze.height) then
					local cell = map[position1]
					if fn(cell) then
						table.insert(neighbours, {position1, cell, i})
					end
				end
			end
		end
		return neighbours
	end

	local function Bdirection(n)
		return 2^(n%4)
	end

	local function BreverseDirection(n)
		return 2^((n+2)%4)
	end

	local function chooseOneElement(t)
		if type(t)=='table' then
			if #t == 1 then
				return t[1]
			else
				return t[math.random(1, #t)]
			end
		end
	end

	local cellTestFn = function(cell)
		if maze.finishOnExit then
			return ((type(cell)=='boolean' and cell == false) or (type(cell)=='table' and (cell.type ~= 1 and cell.type ~= 2)))
		else
			return ((type(cell)=='boolean' and cell == false) or (type(cell)=='table' and (cell.type == 0 or cell.type == 3)))
		end
	end

	local genMaze = coroutine.wrap(function()
		local position = {sx, sy}
		local cell = prepareCell(2); map[position] = cell
		map[{fx, fy}] = prepareCell(3)

		while true do
			local neighboursList = neighbours(position, cellTestFn)

			if #neighboursList>0 then
				-- choose one of the neighbours
				local selectedCellInfo = chooseOneElement(neighboursList)
				local selectedCell = selectedCellInfo[2]
				local direction = selectedCellInfo[3]
				local selectedCellPosition = selectedCellInfo[1]

				-- push current cell into stack
				positionStack.push({position, cell})

				-- mark cell as visited
				if type(selectedCell)~='table' then
					selectedCell = prepareCell(1); map[selectedCellPosition] = selectedCell
				else
					selectedCell.type = 4	
				end

				-- connect current cell and selectedCell
				cell.connections = bit.bor(cell.connections, Bdirection(direction))
				selectedCell.connections = bit.bor(selectedCell.connections, BreverseDirection(direction))

				-- is the selectedCell an exit?
				if maze.finishOnExit and type(selectedCell)=='table' and selectedCell.type >= 3 then
					-- do traceback
					while not positionStack.empty() do
						local position, cell = unpack(positionStack.pop())
						if cell and cell.type == 1 then
							cell.type = 5
						end
					end
					break						
				end

				position = selectedCellPosition
				cell = selectedCell
				coroutine.yield(0)
			-- if there's no way to go, let's get back
		    elseif not positionStack.empty() then
		    	position, cell = unpack(positionStack.pop())
				coroutine.yield(2)
		    -- if there's no way to go and there is no solution for this path
			else
				for i=1,100 do
					local selectedCellPosition = {math.random(1, maze.width), math.random(1, maze.height)}
					local selectedCell = map[position]
					if type(selectedCell)~='table' then
						selectedCell = prepareCell(1); map[selectedCellPosition] = selectedCell

						position = selectedCellPosition
						cell = selectedCell
						break
					end
				end
				coroutine.yield(3)
		    end
		end
   		coroutine.yield(1)
	end)

	maze.iterator = function()
		local co = coroutine.create(function()
			for y=1,maze.height do
				for x=1,maze.width do
					local position = {x, y}
					coroutine.yield(position, map[position])
				end
			end
			coroutine.yield()
		end)
		return function()
			local code, pos, cell = coroutine.resume(co)
			return pos, cell
		end
	end

	maze.load = function(data)
		local r, t =  serpent.load(data)
		if r then
			maze.width = t.width
			maze.height = t.height
			maze.entry = t.entry
			maze.exit = t.exit
			maze.finishOnExit = t.finishOnExit
			maze.map.load(t.map)
		end
	end

	maze.save = function()
		return serpent.dump({
			map = maze.map.save(),
			width = maze.width,
			height = maze.height,
			entry = maze.entry,
			exit = maze.exit,
			finishOnExit = maze.finishOnExit,
		})
	end

	maze.generate = genMaze
	return maze
end