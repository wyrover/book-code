local bit = require 'bit'
local stack = require 'utils/stack'
local map2D = require 'pathfinding/map2d'

return function(maze)
	local solver = {}
	local map = maze.map

	local solve = function(startingPoint)
		local startingPoint = startingPoint or {maze.entry.x, maze.entry.y}
		local exitPoint = {maze.exit.x, maze.exit.y}

		local neighbordLocations = {
			[0] = {0, 1},
			[1] = {1, 0},
			[2] = {0, -1},
			[3] = {-1, 0},
		}

		local function neighbours(position0, fn)
			local neighbours = {}
			local currentCell = map[position0]
			if type(currentCell)=='table' then
				local connections = currentCell.connections
				if type(fn)=="function" then
					for i=0,3 do
						if bit.band(connections, 2^i) >= 1 then
							local neighbourLocation = neighbordLocations[i] 
							local position1 = {position0[1] + neighbourLocation[1], position0[2] + neighbourLocation[2]}
							if (position1[1]>=1 and position1[1] <= maze.width and position1[2]>=1 and position1[2] <= maze.height) then
								if fn(map[position1], position1) then
									table.insert(neighbours, position1)
								end
							end
						end
					end
				end
			else
				print(position0[1], position0[2], currentCell)
			end
			return neighbours
		end


		return coroutine.wrap(function()
			local path = stack()
			local currentPosition = startingPoint
			local visitedCells = map2D(false)

			local cellTestFn = function(cell, position)
				return (cell.type >= 1) and (not visitedCells[position])
			end

			local function findPath(currentPosition)
				visitedCells[currentPosition] = true
				path.push(currentPosition)

				while true do
					local currentCell = maze.map[currentPosition]

    				-- is this an exit?
    				if currentCell and (currentCell.type == 3 or currentCell.type == 4) then
						break
    				else
    					local possibleCells = neighbours(currentPosition, cellTestFn)

    					-- is this a dead end?
    					if #possibleCells>0 then
    						currentPosition = possibleCells[1]
							
							visitedCells[currentPosition] = true
							path.push(currentPosition)

   							coroutine.yield(false, currentPosition, #possibleCells)
    					elseif not path.empty() then
    						currentPosition = path.pop()
    						--coroutine.yield(false, currentPosition)
    					else
    						break
    					end
    		    	end
    			end
		    end
		    
		    xpcall(findPath, function(msg)
				print(msg)
				print(debug.traceback())
		    end,currentPosition)

			coroutine.yield(true, path)
		end)
	end


	solver.solve = solve
	return solver
end