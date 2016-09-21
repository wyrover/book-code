local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'
local shader = require 'SDL/shader'
local vertexBuffer = require 'SDL/vertex_buffer'
local frameBuffer = require 'SDL/frame_buffer'
local renderBuffer = require 'SDL/render_buffer'

local window = {
	x = 128,
	y = 128,
	width = 1024,
	height = 786,
	bpp = 32,
	flags = SDL.SDL_OPENGL,
	caption = 'PackPub Example',
	class = 'Example',
	fullScreen = false,
}

if window.fullScreen then
	window.flags = bit.bor(window.flags, SDL.SDL_FULLSCREEN)
end

local function createVertexArray(target)
	local vertexArrayObject = {}
	local vertexArrayObjectID = (gl.GenVertexArrays(1))[1]
	vertexArrayObject.raw = vertexArrayObjectID

	local target = target or gl_enum.GL_ARRAY_BUFFER
	local deleted = false
		
	vertexArrayObject.enableAttrib = function(index)
		gl.EnableVertexAttribArray(index)
	end

	vertexArrayObject.disableAttrib = function(index)
		gl.DisableVertexArray(index)
	end
		
	local function use()
		gl.BindVertexArray(vertexArrayObjectID)
	end
	vertexArrayObject.use = use

	vertexArrayObject.map = function(index, buffer, size)
		local size = size or 0
		gl.BindBuffer(target, buffer.raw)
		gl.VertexAttribPointer(index, size, false, 0)
	end

	vertexArrayObject.draw = function(drawType, first, count)
		local first = first or 0
		local count = count or 1
		use()
		gl.DrawArrays(drawType, first, count)
	end

	vertexArrayObject.delete = function()
		if not deleted then
			gl.DeleteVertexArrays({vertexArrayObjectID})
			deleted = true
		end
	end

	setmetatable(vertexArrayObject, {
		__gc = vertexArrayObject.delete,
	})
	table.proxy(vertexArrayObject)
	return vertexArrayObject
end

local screenAspectRatio = window.width/window.height
local textures = {}
local shaders = {}
local buffers = {}
local vertexArrays = {}

local function prepareScene()
	-- prepare shader
	shaders.basic = shader.load('../../common/shaders/basic_vbo')
	shaders.basic.uniform.define {
		{name = 'textured', type = {'i'}},
		{name = 'diffuseTexture', type = {'i'}},
	}

	shaders.basic.use()
	local activeTextureUnit = 0

	textures.texture1.use(activeTextureUnit)
	shaders.basic.uniform.textured = 1
	shaders.basic.uniform.diffuseTexture = activeTextureUnit

	-- bind vertex attributes
	shaders.basic.bind {
		[0] = 'VertexPosition',
		[1] = 'VertexColor',
		[2] = 'VertexTexCoord',
	}

	-- prepare vertex buffers
	buffers.vertex = vertexBuffer.new()
	buffers.color = vertexBuffer.new()
	buffers.texCoord = vertexBuffer.new()

	-- fills vertex buffers with data
	-- x, y
	buffers.vertex.data {
		0, 1,			-- vertex #1
		-1, -1,			-- vertex #2
		1, -1,			-- vertex #3
	}

	-- r, g, b, a
	buffers.color.data {
		1, 0, 0, 1,		-- vertex #1
		0, 1, 0, 1,		-- vertex #2
		0, 0, 1, 1,		-- vertex #3
	}

	-- u, v
	buffers.texCoord.data {
		0.5, 0,			-- vertex #1
		0, 1,			-- vertex #2
		1, 1,			-- vertex #3
	}

	-- prepare vertex array
	vertexArrays.triangle = createVertexArray()
	vertexArrays.triangle.use()
	-- maps vertex buffer to vertex attributes with these parameters (attribute_index, vertex_buffer_object, elements_per_primitive)
	vertexArrays.triangle.map(0, buffers.vertex, 2)
	vertexArrays.triangle.map(1, buffers.color, 4)
	vertexArrays.triangle.map(2, buffers.texCoord, 2)

	-- enable all vertex attributes
	vertexArrays.triangle.enableAttrib(0)
	vertexArrays.triangle.enableAttrib(1)
	vertexArrays.triangle.enableAttrib(2)
end

local function drawScene()
	shaders.basic.use()

	vertexArrays.triangle.draw(gl_enum.GL_TRIANGLES, 0, 3)
end

local events = {
	startup = function()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()
		assert(gl.IsSupported("GL_VERSION_3_0 GL_ARB_vertex_shader GL_ARB_fragment_shader"), "Your GPU doesn't support GLSL 3.3")

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		gl.ClearColor(0.1,0.1,0.1,1)

		textures.texture1 = texture.load('../../common/assets/lena_std.png')

		prepareScene()
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))

		drawScene()

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	cleanup = function()
	end,
}

app.main(window, events)

