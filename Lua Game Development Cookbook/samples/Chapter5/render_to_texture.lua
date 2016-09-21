local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'
local shader = require 'SDL/shader'
local vertexBuffer = require 'SDL/vertex_buffer'
local vertexArray = require 'SDL/vertex_array'

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

local function createFrameBuffer(target)
	local frameBufferObject = {}
	local frameBufferObjectID = (gl.GenFrameBuffers(1))[1]
	frameBufferObject.raw = frameBufferObjectID

	local target = target or gl_enum.GL_FRAMEBUFFER
	frameBufferObject.target = target

	frameBufferObject.delete = function()
		gl.BindFramebuffer(target, 0)
		gl.DeleteFramebuffers(frameBufferObjectID)
	end

	frameBufferObject.use = function()
		gl.BindFramebuffer(target, frameBufferObjectID)
	end

	frameBufferObject.unUse = function()
		gl.BindFramebuffer(target, 0)
	end

	frameBufferObject.status = function()
		return gl.CheckFramebufferStatus(gl_enum.GL_DRAW_FRAMEBUFFER)
	end

	frameBufferObject.statusText = function()
		local status = frameBufferObject.status()
		local msg = {
			[gl_enum.GL_FRAMEBUFFER_UNDEFINED] = "Undefined",
			[gl_enum.GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT] = "Incomplete attachment",
			[gl_enum.GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT] = "Missing attachment",
			[gl_enum.GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER] = "Incomplete Draw buffer",
			[gl_enum.GL_FRAMEBUFFER_UNSUPPORTED] = "Unsupported",
			[gl_enum.GL_FRAMEBUFFER_COMPLETE] = "Complete",
		}
		return msg[status] or 'Unknown state'
	end

	setmetatable(frameBufferObject, {
		__gc = frameBufferObject.delete,
	})
	table.proxy(frameBufferObject)
	return frameBufferObject
end

local function createRenderBuffer(target)
	local renderBufferObject = {}
	local renderBufferObjectID = (gl.GenRenderBuffers(1))[1]
	renderBufferObject.raw = renderBufferObjectID

	local target = target or gl_enum.GL_RENDERBUFFER
	renderBufferObject.target = target

	renderBufferObject.delete = function()
		renderBufferObject.unUse()
		gl.DeleteFramebuffers(renderBufferObjectID)
	end

	renderBufferObject.use = function()
		gl.BindRenderbuffer(target, renderBufferObjectID)
	end

	renderBufferObject.unUse = function()
		gl.BindRenderbuffer(target, 0)
	end

	renderBufferObject.setAttachment = function(frameBuffer, attachment, internalFormat, width, height)
		renderBufferObject.use()
		gl.RenderbufferStorage(target, internalFormat, width, height)
		gl.FramebufferRenderbuffer(frameBuffer.target, attachment, target, renderBufferObjectID)
	end

	setmetatable(renderBufferObject, {
		__gc = renderBufferObject.delete,
	})
	table.proxy(renderBufferObject)
	return renderBufferObject
end

local screenAspectRatio = window.width/window.height
local shaders = {}
local buffers = {}
local vertexArrays = {}
local textures = {}

local function copyFramebufferContent()
	local srcLevel = 0
	local srcX, srcY, srcZ = 0, 0, 0
	local srcWidth, srcHeight = window.width, window.height
	local srcDepth = 1

	local destLevel = 0
	local destX, destY, destZ = 0, 0, 0

	gl.CopyImageSubData(
		buffers.render.raw, buffers.render.target,
		srcLevel, 
		srcX, srcY, srcZ,
		textures.frame.raw, textures.frame.target,
		destLevel, 
		destX, destY, destZ, 
		srcWidth, srcHeight, srcDepth
	)
end

local function prepareScene()
	-- prepare shaders
	shaders.basic = shader.load('../../common/shaders/basic_vbo')
	shaders.framebuffer = shader.load('../../common/shaders/framebuffer')

	-- ### Basic shader setup
	shaders.basic.use()
	shaders.basic.uniform.define {
		{name = 'textured', type = {'i'}},
		{name = 'diffuseTexture', type = {'i'}},
	}

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

	-- ### Framebuffer shader setup
	shaders.framebuffer.use()
	shaders.framebuffer.uniform.define {
		{name = 'textured', type = {'i'}},
		{name = 'diffuseTexture', type = {'i'}},
		{name = 'useVertexColors', type = {'i'}},
	-- prepare model-view matrix 3x3 for 2D operations
		{name = 'modelViewMatrix', type = {'f', 3, 3, true}},
	}

	shaders.framebuffer.uniform.useVertexColors = 0
	shaders.framebuffer.uniform.textured = 1
	shaders.framebuffer.uniform.diffuseTexture = activeTextureUnit
	--[[
		Setup transformation for rotation by 5 degrees and scaling down by 5%.
		This will result in nice whirlpool effect.

		You can obtain matrix with Octave or MATLAB application	by using whis code:
		    pi180 = (pi/180);
			scale = 0.95;
			angle = 5 * pi180;
			A = [cos(angle) sin(angle) 0; -sin(angle) cos(angle) 0; 0 0 1];
			B = [scale 0 0; 0 -scale 0; 0 0 1];
			A*B

		You can even use octave online at: http://octave-online.net/
	--]]
	shaders.framebuffer.uniform.modelViewMatrix = {
		0.9464,		-0.0828,	0,
		-0.0828,	-0.9464,	0,
		0,			0,			1.0000,
	}

	-- bind vertex attributes
	shaders.framebuffer.bind {
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
		-1, 1,			-- vertex #1
		-1, -1,			-- vertex #2
		1, -1,			-- vertex #3
		1, 1,			-- vertex #3
	}

	-- r, g, b, a
	buffers.color.data {
		1, 0, 0, 1,		-- vertex #1
		0, 1, 0, 1,		-- vertex #2
		0, 0, 1, 1,		-- vertex #3
		1, 1, 0, 1,		-- vertex #3
	}

	-- u, v
	buffers.texCoord.data {
		0, 0,			-- vertex #1
		0, 1,			-- vertex #2
		1, 1,			-- vertex #3
		1, 0,			-- vertex #3
	}

	-- prepare vertex array
	vertexArrays.quad = vertexArray.new()
	vertexArrays.quad.use()
	-- maps vertex buffer to vertex attributes with these parameters (attribute_index, vertex_buffer_object, elements_per_primitive)
	vertexArrays.quad.map(0, buffers.vertex, 2)
	vertexArrays.quad.map(1, buffers.color, 4)
	vertexArrays.quad.map(2, buffers.texCoord, 2)

	-- enable all vertex attributes
	vertexArrays.quad.enableAttrib(0)
	vertexArrays.quad.enableAttrib(1)
	vertexArrays.quad.enableAttrib(2)

	-- prepare frame and render buffer objects
	buffers.frame = createFrameBuffer()
	buffers.render = createRenderBuffer()

	-- use frame buffer object and set color attachment
	buffers.frame.use()
	buffers.render.setAttachment(buffers.frame, gl_enum.GL_COLOR_ATTACHMENT0, gl_enum.GL_RGBA8, window.width, window.height)

	-- prepare texture that will contain frame-buffer content
	textures.frame = texture.empty(window.width, window.height)

	-- initialize the first render into texture
	shaders.basic.use()

	-- turn on frame-buffer capture
	buffers.frame.use()
	textures.texture1.use(0)
	vertexArrays.quad.draw(gl_enum.GL_QUADS, 0, 4)

	-- turn off frame-buffer capture
	buffers.frame.unUse()

	copyFramebufferContent()
end

local function drawScene()
	shaders.framebuffer.use()

	-- turn on frame-buffer capture
	buffers.frame.use()
	-- use previous version of the frame-buffer
	textures.frame.use(0)
	vertexArrays.quad.draw(gl_enum.GL_QUADS, 0, 4)

	-- turn off frame-buffer capture
	buffers.frame.unUse()

	copyFramebufferContent()

	shaders.framebuffer.use()
	textures.frame.use(0)
	vertexArrays.quad.draw(gl_enum.GL_QUADS, 0, 4)
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

