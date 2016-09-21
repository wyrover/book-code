local app = require 'sdl_app'

local window = {
	x = 128,
	y = 128,
	width = 640,
	height = 480,
	bpp = 0,
	flags = 0,
	caption = 'PackPub Example',
	class = 'Example',
}

local valueTypes = {
	bool = function(value)
		return ((value==1) and 'true') or 'false'
	end,
	KBsize = function(value)
		return ("%d kB"):format(tonumber(value))
	end,
	pixelFormat = function(value)
		return ([[

	Bits per pixel: %-2d
	Bytes per pixel: %-2d
	Precision loss of each color component:
		R: %-2d G: %-2d B:%-2d A: %-2d
	Binary left shift of each color component:
		R: %-2d G: %-2d B:%-2d A: %-2d
	Binary mask of each color component:
		R: 0x%08X G: 0x%08X B: 0x%08X A: 0x%08X
	Color key: 0x%08X
	Alpha value: %-2d
]]):format(
			value.BitsPerPixel,
			value.BytesPerPixel,
			value.Rloss, value.Gloss, value.Bloss, value.Aloss,
			value.Rshift, value.Gshift, value.Bshift, value.Ashift,
			value.Rmask, value.Gmask, value.Bmask, value.Amask,
			value.colorkey,
			value.alpha
		)
	end,
	string = function(value)
		return tostring(value)
	end,
}

local infoFields = {
	{function()
		local str = string.rep(" ", 256)
		return SDL.SDL_VideoDriverName(str, #str)
	end, 'Video Driver name', 'string'},
	{'hw_available', 'Hardware surface support', 'bool'},
	{'wm_available', 'Window Manager present', 'bool'},
	{'blit_hw', 'HW to HW blit acceleration support', 'bool'},
	{'blit_hw_CC', 'HW to HW blit with colorkey acceleration support', 'bool'},
	{'blit_hw_A', 'HW to HW blit with alpha channel acceleration support', 'bool'},
	{'blit_sw', 'SW to HW blit acceleration suppport', 'bool'},
	{'blit_sw_CC', 'SW to HW blit with colorkey acceleration support', 'bool'},
	{'blit_sw_A', 'SW to HW blit with alpha channel acceleration support', 'bool'},
	{'blit_fill', 'Color fill acceleration support', 'bool'},
	{'video_mem', 'Video memory size', 'KBsize'},
	{'vfmt', 'Current pixel format', 'pixelFormat'},
}

local events = {
	startup = function()
		local videoInfo = SDL.SDL_GetVideoInfo()

		-- shows formated information about SDL video device
		for _, fieldData in ipairs(infoFields) do
			local fieldValueTransformFn = valueTypes[fieldData[3]]
			local fieldValue = ''

			if type(fieldData[1])=='string' then
				local fieldName = fieldData[1]
				local rawFieldValue = videoInfo[fieldName]
				fieldValue = fieldValueTransformFn(rawFieldValue)
			elseif type(fieldData[1])=='function' then
				fieldValue = fieldValueTransformFn((fieldData[1])())
			end
			print(('%-60s: %s'):format(fieldData[2], fieldValue))
		end
	end,
}

app.main(window, events)

