local pqueue = require 'utils/pqueue'

local function dprint(fmt, ...)
	local f = io.open('debug.log','a')
	if f then
		if #({...}) > 0 then
			f:write(fmt:format(...))
		else
			f:write(fmt)
		end 
		f:write("\n")
		f:close()
	end
end

return function(map)
	local solver = {}
	local mapCells = map.map

	local solve = function(startingPoint, exitPoint)
		local start = startingPoint or {map.entry.x, map.entry.y}
		local goal = exitPoint or {map.exit.x, map.exit.y}

		return coroutine.wrap(function()
			local cellCache = {}
			local function cantorPair(k1, k2)
				return 0.5 * (k1 + k2) * ((k1 + k2) + 1) + k2
			end
			local function storeCells(...)
				for _, elm in ipairs {...} do
					cellCache[cantorPair(elm[1], elm[2])] = elm
				end
			end
			local function queryCell(p)
				local cp = cantorPair(p[1], p[2])
				local cell = cellCache[cp]
				if not cell then
					cell = p
					cellCache[cp] = cell
				end
				return cell
			end

			local directionSet = {
				{-1,-1}, { 0, 1}, { 1, 1},
				{-1, 0},          { 1, 0},
				{ 1,-1}, { 0,-1}, {-1, 1},
			}

			local function neighboursFn(p0)
            	local list = {}
   	        	for _, direction in ipairs(directionSet) do
       	    		local p1 = queryCell({p0[1] + direction[1], p0[2] + direction[2]})
    				coroutine.yield(p1)
            	end
   				coroutine.yield()
			end

            local function neighbours(p0)
            	return coroutine.wrap(function()
            		return neighboursFn(p0)
            	end)
        	end

        	-- Manhattan distance
            local function heuristicCostEstimate(p0, p1)
				return math.abs(p0[1] - p1[1]) + math.abs(p0[2] - p1[2])
            end

            local function cost(p0, p1)
				local cell = mapCells[p1]
				if cell == 0 then
					-- wall cost
					return math.huge
				else
					-- normal step cost
					return 1
				end
            end

            local function reconstructPath(cameFrom, goal)
			    local totalPath = {current}

			    local current = cameFrom[goal]
			    while current do
			        table.insert(totalPath, current)
			        current = cameFrom[current]
			    end
			    return totalPath
            end

			local function findPath()
				local frontier = pqueue()
				local cameFrom = {}
				local costSoFar = {
					[start] = 0,
				}
				frontier[start] = 0
				storeCells(start, goal)

				while not frontier.empty() do
					local current = assert((frontier.min())[1])
					-- are we at goal?
					if current == goal then
						local path = reconstructPath(cameFrom, goal)
						coroutine.yield(true, path)
						return path
					end
					frontier.remove(current)

					for neighbour in neighbours(current) do
						local newCost = costSoFar[current] + cost(current, neighbour)

						if not costSoFar[neighbour] or (newCost < costSoFar[neighbour]) then
							--dprint(("[%d, %d] - [%d, %d] = %0.4f %0.4f %s"):format(current[1], current[2], neighbour[1], neighbour[2], newCost, costSoFar[neighbour] or -1, tostring(frontier[neighbour])))
							costSoFar[neighbour] = newCost
							frontier[neighbour] = newCost + heuristicCostEstimate(goal, neighbour)
							cameFrom[neighbour] = current
						end
					end
					coroutine.yield(false, current)
				end
				coroutine.yield(true, {})
			end

		    xpcall(findPath, function(msg)
				print(msg)
				print(debug.traceback())
		    end)

			coroutine.yield(true, path)
		end)
	end


	solver.solve = solve
	return solver
end