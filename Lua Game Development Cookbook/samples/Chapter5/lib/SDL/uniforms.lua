local gl = require 'luagl'

-- uniform variable management
local function init(shaderProgramID)
 	local uniforms = {}

 	local function getUniformLocation(name)
		local location = gl.GetUniformLocation(shaderProgramID, name)
		--assert(location ~= -1, ('Not active uniform variable (%s)'):format(name))
		return location
	end

	local uniformLocations = {}
	local uniformTypes = {}
	local uniformValues = {}

	local function defineUniform(typeDef)
		local name = typeDef.name
		local uniformType = typeDef.type
		assert(type(uniformType)=='table')
		local location = getUniformLocation(name)
		uniformLocations[name] = location
		uniformTypes[name] = uniformType
	end

	uniforms.define = function(typeDef)
		assert(type(typeDef)=='table')
		if type(typeDef.name)=='string' then
			defineUniform(typeDef)
		-- allows definition of all uniforms withing one table
		elseif #typeDef>0 then
			for _, uniformTypeDef in ipairs(typeDef) do
				defineUniform(uniformTypeDef)
			end
		end
	end

	local function setUniform(k, v)
		local location = uniformLocations[k]
		local uniformType = uniformTypes[k]

		if location == -1 then
			return
		end

		if location and uniformType then
			local baseType = uniformType[1]
			local length = uniformType[2] or 1
				
			if baseType == 'i' then
				if length > 1 and type(v)=='table' then
					gl.Uniformi(location, unpack(v))
				else
					gl.Uniformi(location, v)
				end					
			elseif baseType == 'ui' then
				if length > 1 and type(v)=='table' then
					gl.Uniformui(location, unpack(v))
				else
					gl.Uniformui(location, v)
				end					
			elseif baseType == 'f' then
				if length > 1 and type(v)=='table' then
					local cols = uniformType[3]

					if type(cols)=='number' and cols>1 then
						local rows = length
						local transpose = uniformType[4]
                    
                    	if type(transpose) ~= 'boolean' then
							transpose = false
						end

						gl.UniformMatrix(location, v, rows, cols, transpose)
					else
						gl.Uniformf(location, unpack(v))
					end
				else
					gl.Uniformf(location, v)
				end
			else
				error(('Unknown uniform type (%s)'):format(tostring(baseType)))
			end

			uniformValues[k] = v
		else
			error(('Trying to set undefined uniform variable (%s)'):format(k))
		end
	end

	setmetatable(uniforms, {
		__index = function(_, k)
			local location = uniformLocations[k]
			local uniformType = uniformTypes[k]

			if uniformType then
				if not location then
					location = getUniformLocation(k)
					uniformLocations[k] = location
				end
				return uniformValues[k]
	    	end
	    	return nil
		end,

		__newindex = function(_, k, v)
			assert(type(k)=='string')
			setUniform(k, v)
		end,

		__call = function(_, t)
			if type(t)=='table' then
				for k,v in pairs(t) do
					setUniform(k, v)
				end
			end
		end,
	})

	return uniforms
end

return {
	init = init,
}