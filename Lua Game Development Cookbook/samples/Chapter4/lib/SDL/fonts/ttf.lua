local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'
local atlas = require 'SDL/atlas'

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

local function loadTTF(fname, initialSize, fontFace)
	local obj = {}
	local initialSize = initialSize or 128
	local globalSizeFactor = (1/initialSize*0.5)
	local fontAtlas = atlas.new(16*initialSize, 16*initialSize)

	-- font face index
	local fontFace = fontFace or 0

	local rwop = assert(SDL.SDL_RWFromFile(fname,"rb"), string.format("Can't load file '%s': %s", fname, tostring(SDL.SDL_GetError())))
	local raw = assert(SDL.TTF_OpenFontIndexRW(rwop, 1, initialSize, fontFace), SDL.TTF_GetError())

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

					--glyph.texture = texture.new(surface, true)

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

	obj.print = function(x, y, color, sizeFactor, text, debug)
		assert(type(text)=='string')
		if #text>0 then

			local color = color or {r=1, g=1, b=1, a=1}
			local sizeFactor = globalSizeFactor * (sizeFactor or 1)
			-- pos_x and pos_y store current glyph position on screen
			local pos_x, pos_y = x, y
			
			-- loop through all string characters
			for i=1, #text do
				-- character code
				local code = string.byte(text,i)
				-- obtain cached glyph metrics
				local glyph = obj.glyph(code)
				assert(glyph, ('Glyph not found %d: (%s) -> %s'):format(code, string.sub(text, i, i), text))

				if glyph then
	
		            -- use glyph texture if there's any
					fontAtlas.use()

					local glyphItem = fontAtlas.items[code]

					if type(glyphItem)=='table' then

						local tc_x0, tc_y0, tc_x1, tc_y1 = glyphItem.fX, glyphItem.fY, glyphItem.fX + glyphItem.fWidth, glyphItem.fY + glyphItem.fHeight
						-- set up glyph coordinates
						local glyph_x = pos_x + glyph.minx * sizeFactor
					    local glyph_y = -pos_y + (descent - glyph.maxy) * sizeFactor
						local glyph_width = (glyph.maxx - glyph.minx) * sizeFactor
						local glyph_height = (glyph.maxy - glyph.miny) * sizeFactor

						-- accomodate vertical position to viewport coordinates
						glyph_y = -glyph_y - glyph_height + (height*sizeFactor/2)

						-- draws a rectangle filled with glyph image
						gl.Begin(gl_enum.GL_QUADS)
							gl.Color4f(color.r, color.g, color.b, color.a)

							gl.TexCoord2f(tc_x0, tc_y1)
							gl.Vertex2f(glyph_x, glyph_y)

							gl.TexCoord2f(tc_x1, tc_y1)
							gl.Vertex2f(glyph_x + glyph_width, glyph_y)

							gl.TexCoord2f(tc_x1, tc_y0)
							gl.Vertex2f(glyph_x + glyph_width, glyph_y + glyph_height)

							gl.TexCoord2f(tc_x0, tc_y0)
							gl.Vertex2f(glyph_x, glyph_y + glyph_height)
						gl.End()

						pos_x = pos_x + glyph.advance * sizeFactor
	            	end
	        	end
			end

			fontAtlas.unUse()
		end
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