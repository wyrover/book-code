local gl = require 'luagl'

local function wrapTextureObject(target, texture_id, parentObj)
	local parentObj = parentObj or {}
	
	local deleted = false

	parentObj.raw = texture_id

	parentObj.delete = function()
		if not deleted then
			gl.DeleteTextures({texture_id})
		end
	end
	parentObj.use = function(index)
		if type(index)=="number" and (index >=0) then
			gl.ActiveTexture(gl_enum.GL_TEXTURE0 + index)
		end
		gl.BindTexture(target, texture_id)
	end
	parentObj.unUse = function()
		gl.BindTexture(target, 0)
	end

	setmetatable(parentObj, {
		__gc = parentObj.delete,
	})
	table.proxy(parentObj)

	return parentObj
end

local function createTexture()
	local texture_id = (gl.GenTextures(1))[1]
	gl.BindTexture(gl_enum.GL_TEXTURE_2D, texture_id)
	
	gl.TexParameteri(gl_enum.GL_TEXTURE_2D, gl_enum.GL_TEXTURE_WRAP_S, gl_enum.GL_CLAMP_TO_EDGE)
	gl.TexParameteri(gl_enum.GL_TEXTURE_2D, gl_enum.GL_TEXTURE_WRAP_T, gl_enum.GL_CLAMP_TO_EDGE)
	gl.TexParameteri(gl_enum.GL_TEXTURE_2D, gl_enum.GL_TEXTURE_MIN_FILTER, gl_enum.GL_LINEAR)
	gl.TexParameteri(gl_enum.GL_TEXTURE_2D, gl_enum.GL_TEXTURE_MAG_FILTER, gl_enum.GL_LINEAR)

	return texture_id
end

local function createEmptyTexture(width, height)
    local texture_id = createTexture()

	local texture_target = gl_enum.GL_TEXTURE_2D
	local mipmap_level = 0
	local internal_format = gl_enum.GL_RGBA8
	local border = 0
	local format = gl_enum.GL_RGBA
	local data = 0
	gl.TexImage2D(texture_target, mipmap_levels, internal_format, width, height, border, format, data)
	gl.Finish()

	gl.BindTexture(gl_enum.GL_TEXTURE_2D, 0)
	return wrapTextureObject(texture_target, texture_id, {
		width = width,
		height = height,
		target = texture_target,
	})
end

local function createTextureFromSurface(rawSurfaceObject, transparent)
	if type(rawSurfaceObject)=='table' then
		rawSurfaceObject = rawSurfaceObject.raw
	end
	if (transparent) then
		SDL.SDL_SetAlpha(rawSurfaceObject, 0, 0)
	end

    local texture_id = createTexture()
    local bpp = rawSurfaceObject.format.BitsPerPixel

	local texture_target = gl_enum.GL_TEXTURE_2D
	local mipmap_level = 0
	local internal_format = gl_enum.GL_RGBA
	local width = rawSurfaceObject.w
	local height = rawSurfaceObject.h
	local border = 0
    local format

	-- determine surface image data format
	if bpp == 24 then
    	format = gl_enum.GL_RGB
    elseif bpp == 32 then
    	format = gl_enum.GL_RGBA
    else
    	error('Unsupported image format')
    end

	local data = rawSurfaceObject.pixels
	gl.TexImage2D(texture_target, mipmap_levels, internal_format, width, height, border, format, data)
	gl.Finish()

	gl.BindTexture(gl_enum.GL_TEXTURE_2D, 0)
	return wrapTextureObject(texture_target, texture_id, {
		width = width,
		height = height,
		target = texture_target,
	})
end

local function createTextureFromImageFile(fname, transparent)
	local rawSurfaceObject = assert(SDL.IMG_Load(fname))
	return createTextureFromSurface(rawSurfaceObject, transparent)
end


return {
	new = createTextureFromSurface,
	empty = createEmptyTexture,
	load = createTextureFromImageFile,
}