local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'
local atlas = require 'SDL/atlas'
local vertexBuffer = require 'SDL/vertex_buffer'
local vertexArray = require 'SDL/vertex_array'
local shader = require 'SDL/shader'
local matrixModule = require 'utils/matrix'
local matrix = matrixModule.new
local matrixBase = matrixModule.new()

local function pixelFormatABGR8888()
	local pf = SDL.SDL_PixelFormat_local()
	pf.BitsPerPixel = 32
	pf.BytesPerPixel = 4
	pf.Rmask = 0x000000FF
	pf.Gmask = 0x0000FF00
	pf.Bmask = 0x00FF0000
	pf.Amask = 0xFF000000
	pf.Rloss = 0
	pf.Gloss = 0
	pf.Bloss = 0
	pf.Aloss = 0
	pf.Rshift = 0
	pf.Gshift = 0
	pf.Bshift = 0
	pf.Ashift = 0
	pf.colorkey = 0
	pf.alpha = 255
	pf.palette = nil
	return pf
end

local function prepareFontShader(fontTextureUnit)
	local shaderObject
	-- prepare shaders
	shaderObject = shader.load('../../common/shaders/font')

	-- ### Font shader setup
	shaderObject.use()
	shaderObject.uniform.define {
		{name = 'baseColor', type = {'f', 4}},
		{name = 'diffuseTexture', type = {'i'}},
		{name = 'modelViewMatrix', type = {'f', 4, 4, true}},
		{name = 'projectionMatrix', type = {'f', 4, 4, true}},
		{name = 'texMatrix', type = {'f', 3, 3, true}},
	}

	fontTextureUnit = fontTextureUnit or 1
	shaderObject.uniform.baseColor = {1,1,1,1}
	shaderObject.uniform.diffuseTexture = fontTextureUnit
	shaderObject.uniform.modelViewMatrix = matrix()
	shaderObject.uniform.projectionMatrix = matrix()
	shaderObject.uniform.texMatrix = {
		1,0,0,
		0,1,0,
		0,0,1,
	}

	-- bind vertex attributes
	shaderObject.bind {
		[0] = 'VertexPosition',
		[1] = 'VertexTexCoord',
	}
	return shaderObject
end

local function preapreGlyphQuad()
	local buffers = {}
	-- prepare vertex buffers
	buffers.vertex = vertexBuffer.new()
	buffers.texCoord = vertexBuffer.new()

	-- fills vertex buffers with data
	-- x, y
	buffers.vertex.data {
		-0.5, 0.5,  0,		-- vertex #1
		-0.5, -0.5, 0,		-- vertex #2
		0.5, -0.5,  0,		-- vertex #3
		0.5, 0.5,   0,		-- vertex #3
	}

	-- u, v
	buffers.texCoord.data {
		0, 0,			-- vertex #1
		0, 1,			-- vertex #2
		1, 1,			-- vertex #3
		1, 0,			-- vertex #3
	}

	-- prepare vertex array
	local quad = vertexArray.new()
	quad.use()
	-- maps vertex buffer to vertex attributes with these parameters (attribute_index, vertex_buffer_object, elements_per_primitive)
	quad.map(0, buffers.vertex, 3)
	quad.map(1, buffers.texCoord, 2)

	-- enable all vertex attributes
	quad.enableAttrib(0)
	quad.enableAttrib(1)

	return {
		buffers = buffers,
		quad = quad,

		draw = function()
			quad.draw(gl_enum.GL_QUADS, 0, 4)
		end,
	}
end

local function loadTTF(fname, initialSize, fontFace)
	local obj = {}
	local initialSize = initialSize or 128
	local globalSizeFactor = (1/initialSize*0.5)
	local fontAtlas = atlas.new(16*initialSize, 16*initialSize)

	-- font face index
	local fontFace = fontFace or 0

	local rwop = assert(SDL.SDL_RWFromFile(fname,"rb"), string.format("Can't load file '%s': %s", fname, tostring(SDL.SDL_GetError())))
	local raw = assert(SDL.TTF_OpenFontIndexRW(rwop, 1, initialSize, fontFace), SDL.TTF_GetError())

	local fontTextureUnit = 1
	local shader = prepareFontShader(fontTextureUnit)
	local quad = preapreGlyphQuad()

	local kerning = matrix()
	local kerningInv = matrix()

	-- font global parameters
	local ascent = SDL.TTF_FontAscent(raw); obj.ascent = ascent
	local descent = SDL.TTF_FontDescent(raw); obj.descent = descent
	local height = SDL.TTF_FontHeight(raw); obj.height = height
	local lineskip = SDL.TTF_FontLineSkip(raw); obj.lineskip = lineskip
	local fontFaces = SDL.TTF_FontFaces(raw)

	-- glyph info cache
	local glyphs = {}; obj.glyphs = glyphs

	-- shortcut function for glyph metrics
	local function glyphInfo(code)
		return SDL.TTF_GlyphMetrics(raw, code)
	end

	-- helper function to obtain glyph metrics and a texture
	obj.glyph = function(code, color)
		if not color then
			color = SDL.SDL_Color_local()
			color.r, color.g, color.b, color.a = 255, 255, 255, 255
		end
		local glyph = glyphs[code]

		if glyph == nil then
			-- render one glyph with transparency and antialiasing
			local font_surface = SDL.TTF_RenderGlyph_Blended(raw, code, color)

			if font_surface then
				if type(glyph)~="table" then
					-- obtain and store glyph parameters
					local _a, minx, maxx, miny, maxy, advance = glyphInfo(code)

	            	-- convert glyph surface into more suitable format
					local surface = (surface.wrap(font_surface)).convert(pixelFormatABGR8888())
					glyph = {
						minx = minx,
						maxx = maxx,
						miny = miny,
						maxy = maxy,
						advance = advance,
						width = maxx - minx,
						height = maxy - miny,
					}
					fontAtlas.add(code, surface, true)
					glyphs[code] = glyph
				end
                return glyph
        	end
    	elseif glyph == false then
	    	return false, "Glyph disabled"
    	else
    		return glyph
    	end
	end

	-- drawGlyph - draws a glyph (textured rectangle) and applies specified matrix transformation on all vertices as well as texture coordinates
	-- Parameters:
	-- localMatrix - corresponds to the transformation matrix for current glyph
	-- globalMatrix - contains transformation matrix for whole text (usually used to place the text at desired position, scale it, ...)
	-- code - character code
	-- color - glyph color defined as a table with r,g,b,a elements e.g.: {r = 1.0, g = 1.0, b = 1.0, a = 1.0}

	local function drawGlyph(localMatrix, globalMatrix, code, color)
		-- obtain cached glyph metrics
		local glyph = obj.glyph(code)
		assert(glyph, ('Glyph not found %d'):format(code))

		if glyph then
			local glyphItem = fontAtlas.items[code]

			if type(glyphItem)=='table' then
				-- cache glyph texture matrix
				local glyphTexMatrix = glyphItem.texMatrix
				if not glyphTexMatrix then
					-- glyph texture coordinates
			        local tx, ty = glyphItem.x, glyphItem.y
			        local tw, th = glyphItem.width, glyphItem.height

					glyphTexMatrix = {
						tw, 0,  tx,
						0,  th, ty,
						0,  0,  1,
					}
					glyphItem.texMatrix = glyphTexMatrix
				end

				local sx, sy = glyph.width, glyph.height
				local lx, ly = sx*0.5 + glyph.minx, -sy*0.5 + glyph.maxy - initialSize*0.5 - 2*descent

		        local glyphMatrix = 
		        	globalMatrix
		        	* localMatrix
		        	* matrixBase.translate(lx, ly, 0)
		        	* matrixBase.scale(sx, sy, 1)

		        -- upload matrices into GPU via uniform variables
				shader.uniform.texMatrix = glyphTexMatrix 
				shader.uniform.modelViewMatrix = glyphMatrix					

				shader.uniform.baseColor = {color.r, color.g, color.b, color.a}

				quad.draw()
				return glyph.advance
           	end
       	end
	end
	
	obj.printM = function(fontMatrix, color, text)
		assert(type(text)=='string')
		if #text>0 then
			shader.use()

			local color = color or {r=1, g=1, b=1, a=1}
			-- setup initial matrices
    		local localMatrix = matrix()
    		fontMatrix = fontMatrix or matrix()

            -- use glyph texture if there's any
			fontAtlas.use(fontTextureUnit)
			
			-- loop through all string characters
			for i=1, #text do
				-- character code
				local code = string.byte(text,i)

				local advance = drawGlyph(localMatrix, fontMatrix, code, color)

				-- advance current position for the next character
				if advance then
	       			localMatrix = localMatrix *
    	   				(kerning * matrixBase.translate(advance, 0, 0) * kerningInv)
		    	end
			end

			fontAtlas.unUse()
		end
	end
	local printM = obj.printM

	obj.print = function(x, y, color, sizeFactor, text)
   		local fontMatrix = matrixBase.translate(x, y, 0) * matrixBase.scale(sizeFactor, sizeFactor, 1)
   		printM(fontMatrix, color, text)
	end

	local deleted = false

	local delete = function()
		if not deleted then
			fontAtlas.delete()
			SDL.TTF_CloseFont(raw)
			deleted = true
		end
	end
	obj.delete = delete

	setmetatable(obj, {
		__gc = delete
	})

	table.proxy(obj)
	return obj
end

return {
	load = loadTTF,
}