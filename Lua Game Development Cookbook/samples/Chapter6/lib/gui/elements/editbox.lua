local BIT = require 'bit'
-- LuaJIT compatible bit functions
local bit = BIT.bit

local matrixModule = require 'utils/matrix'
local matrix = matrixModule.new
local matrixBase = matrixModule.new()

-- useful shortcuts for matrix transformations
local R,T,S = matrixBase.rotate, matrixBase.translate, matrixBase.scale

local defaultProperties = {
	textColor = {r=0,g=0,b=0,a=1},
	caption = 'Default caption',
	maxLength = 24,
	locale = os.setlocale(),
	keyRepeatDelay = 500,
	keyRepeatInterval = 100,
}

local function createElement(interface, def)
	local obj = interface.abstract(def)

	local properties = obj.properties or {}
	local children = obj.children or {}

	local shader = interface.shader
	local shapes = interface.shapes
	local gprint = interface.print

	local uniforms = shader.uniform

	local caret = {
		pos = 0,
		visible = false,
	}; obj.caret = caret

	-- default values
	for name, default in pairs(defaultProperties) do
		if type(properties[name])=='nil' then
			properties[name] = default
		end
	end

	properties.draggable = false

	-- functions for caret movement and text deletion
	local keyboardFn = {
		[SDL.SDLK_LEFT] = function()
			if caret.pos>0 then
				caret.pos = caret.pos - 1
			end
		end,
		[SDL.SDLK_RIGHT] = function()
			if caret.pos<properties.caption then
				caret.pos = caret.pos + 1
			end
		end,
		[SDL.SDLK_HOME] = function()
			caret.pos = 0
		end,
		[SDL.SDLK_END] = function()
			caret.pos = #properties.caption
		end,
		[SDL.SDLK_DELETE] = function()
			if caret.pos < #properties.caption then
				local tmpCaption = {}
				table.insert(tmpCaption, string.sub(properties.caption, 1, caret.pos))
				table.insert(tmpCaption, string.sub(properties.caption, caret.pos+2))
				properties.caption = table.concat(tmpCaption)
			end
		end,
		[SDL.SDLK_BACKSPACE] = function()
			if caret.pos > 0 then
				local tmpCaption = {}
				table.insert(tmpCaption, string.sub(properties.caption, 1, caret.pos-1))
				table.insert(tmpCaption, string.sub(properties.caption, caret.pos+1))
				properties.caption = table.concat(tmpCaption)
				caret.pos = caret.pos - 1
			end
		end,
	}

	local keycodeTranslations = {
	-- standard locale for en-US keyboard layout
		['C'] = {
			shift = {
				[0x31] = 0x21, [0x32] = 0x40, [0x33] = 0x23,
				[0x34] = 0x24, [0x35] = 0x25, [0x36] = 0x5E,
				[0x37] = 0x26, [0x38] = 0x2A, [0x39] = 0x28,
				[0x30] = 0x29, [0x60] = 0x7E, [0x2D] = 0x5F,
				[0x3D] = 0x2B, [0x5B] = 0x7B, [0x5D] = 0x7D,
				[0x5C] = 0x7C, [0x3B] = 0x3A, [0x27] = 0x22,
				[0x2C] = 0x3C, [0x2E] = 0x3E, [0x2F] = 0x3F,
			},
			keypad = {
				[SDL.SDLK_KP_DIVIDE] = 0x2F,
				[SDL.SDLK_KP_MULTIPLY] = 0x2A,
				[SDL.SDLK_KP_MINUS] = 0x2D,
				[SDL.SDLK_KP_PLUS] = 0x2B,
				-- period character on numpad is locale-dependent!
				[SDL.SDLK_KP_PERIOD] = 0x2E,
			},
		},
	}

	local band = bit.band

	--[[
		determine if pressed key is printable along
		with optional translation
	--]]
	local function isPrintable(key, mod)
		local key0 = key
		local keycodeTranslation = keycodeTranslations[properties.locale]
		-- defaults to C locale
		if not keycodeTranslation then
			keycodeTranslation = keycodeTranslations['C']
		end

		if key0 >= 0x20 and key0 < 0x7F then
			if band(mod, SDL.KMOD_SHIFT)>0 or band(mod, SDL.KMOD_CAPS)>0 then
				-- big letters
				if key0>=0x61 and key0<= 0x7A then
					key = key0 - 0x20
				-- alternative characters for various keys
				elseif keycodeTranslation.shift[key0] then
					key = keycodeTranslation.shift[key0]
				end
			end
			return key
		elseif key0 >= 0x100 and key0 <= 0x10E and band(mod, SDL.KMOD_NUM)>0 then
			-- numpad number keys
			if key0 <= 0x109 then
				return key0 - 0xD0
			-- numpad operator keys
			elseif keycodeTranslation.keypad[key0] then
				return keycodeTranslation.keypad[key0]
			end
		end
		return false
	end

	local function processKeyboard(key, mod)
		local key0 = isPrintable(key, mod)
		-- insert character into caption at caret position
		if key0 and #properties.caption < properties.maxLength then
			local tmpCaption = {}
			table.insert(tmpCaption, string.sub(properties.caption, 1, caret.pos))
			table.insert(tmpCaption, string.char(key0))
			table.insert(tmpCaption, string.sub(properties.caption, caret.pos+1))
			properties.caption = table.concat(tmpCaption)
			caret.pos = caret.pos + 1
		else
		-- caret movement
			local kFn = keyboardFn[key]
			if type(kFn)=="function" then
				kFn()
			end
		end
	end

	obj.addSignal('focus', function(self)
		caret.pos = 0
		caret.visible = true
		-- tunrn on key repetition
		SDL.SDL_EnableKeyRepeat(properties.keyRepeatDelay, properties.keyRepeatInterval)
    end)

	obj.addSignal('lostFocus', function(self)
		caret.visible = false
		-- disable key repetition
		SDL.SDL_EnableKeyRepeat(0, 0)
	end)

	obj.addSignal('draw', function(self, modelviewMatrix)
		local fontMatrix0 = modelviewMatrix * T(-self.parent.properties.width/2 + 8, 8, 1) * S(0.15, 0.15, 1)

		gprint(fontMatrix0, properties.textColor, properties.caption)
	end)

	obj.addSignal('mouseButtonDown', function(self)
	end)

	obj.addSignal('mouseButtonUp', function(self)
	end)

	obj.addSignal('keyDown', function(self, key, mod)
		if obj.hasFocus() then
			processKeyboard(key, mod)
		end
	end)

	return obj
end

return {
	new = createElement,
}