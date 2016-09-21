local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'

local window = {
	x = 128,
	y = 128,
	width = 640,
	height = 480,
	bpp = 32,
	flags = SDL.SDL_OPENGL,
	caption = 'PackPub Example',
	class = 'Example',
	fullScreen = false,
}

if window.fullScreen then
	window.flags = bit.bor(window.flags, SDL.SDL_FULLSCREEN)
end

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

					glyph.texture = texture.new(surface, true)

					glyphs[code] = glyph
				end
	
                return glyph
			else
				glyphs[code] = false
		    	return false, "Glyph unavailable"
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
	
	            -- use glyph texture if there's any
		    	if glyph.texture then
					glyph.texture.use()
				end

				-- each glyph uses its own texture, therefore texture coordinates are always static
				local tc_x0, tc_y0, tc_x1, tc_y1 = 0, 0, 1, 1
				-- set up glyph coordinates
				local glyph_x = pos_x + glyph.minx * sizeFactor
			    local glyph_y = -pos_y + (descent - glyph.maxy) * sizeFactor
				local glyph_width = (glyph.maxx - glyph.minx) * sizeFactor
				local glyph_height = (glyph.maxy - glyph.miny) * sizeFactor

				-- accomodate vertical position to viewport coordinates
				glyph_y = -glyph_y - glyph_height - (height*sizeFactor/2)

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

				-- draws a rectangle around glyph
				if debug then
					gl.Disable(gl_enum.GL_TEXTURE_2D)
					gl.Begin(gl_enum.GL_LINE_LOOP)
						gl.Color4f(1, 0, 0, 1)

						gl.Vertex2f(glyph_x, glyph_y)
						gl.Vertex2f(glyph_x + glyph_width, glyph_y)
						gl.Vertex2f(glyph_x + glyph_width, glyph_y + glyph_height)
						gl.Vertex2f(glyph_x, glyph_y + glyph_height)
					gl.End()
					gl.Enable(gl_enum.GL_TEXTURE_2D)
				end
				pos_x = pos_x + glyph.advance * sizeFactor
			end
			-- draws a base line
			if debug then
				gl.Disable(gl_enum.GL_TEXTURE_2D)
				gl.Begin(gl_enum.GL_LINES)
					gl.Color4f(1, 1, 1, 1)
					gl.Vertex2f(x, y)
					gl.Vertex2f(pos_x, y)
				gl.End()
				gl.Enable(gl_enum.GL_TEXTURE_2D)
			end
			gl.BindTexture(gl_enum.GL_TEXTURE_2D, 0)
		end
	end

	local deleted = false
	local delete = function()
		if not deleted then
			for _, glyph in pairs(glyphs) do
				glyph.texture.delete()
			end
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

local fonts = {}

-- debug mode for glyph rendering
local debugMode = false

local events = {
	startup = function()
		SDL.TTF_Init()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		-- enable blending function for alpha transparency
   		gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE_MINUS_SRC_ALPHA)
   		gl.BlendEquation(gl_enum.GL_FUNC_ADD)
   		gl.BlendColor(1,1,1,1)
   		gl.Enable(gl_enum.GL_BLEND)

		gl.ClearColor(0.3,0.3,0.3,1)
		-- initial font size in memory in pixels
		-- greater number results in sharper font but glyphs take more space in GPU memory
		local initialFontSize = 128
		-- loads a font file
		fonts.font1 = assert(loadTTF('../../common/assets/Promocyja096.ttf', initialFontSize))
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))

		-- enable texture rendering
		gl.Enable(gl_enum.GL_TEXTURE_2D)

		-- displays a message
		local color = {r=0.8, g=0.6, b=0.5, a=1}
		-- size factor of the message
		local sizeFactor = 0.75
		fonts.font1.print(-0.8, 0, color, sizeFactor, "Hello world!", debugMode)

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	cleanup = function()
		fonts.font1.delete()
		SDL.TTF_Quit()
	end,
}

app.main(window, events)

