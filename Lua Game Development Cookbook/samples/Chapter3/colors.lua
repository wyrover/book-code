local app = require 'sdl_app'
local surface = require 'SDL/surface'

local floor = math.floor

local function hsl2rgb(h, s, l)
	local r, g, b
	if s == 0 then
		r,g,b = l,l,l -- achromatic
	else
		local p, q
	    local function hue2rgb(t)
			if t < 0 then
				t = t + 1
			end
			if t > 1 then
				t = t - 1
			end

			if t < 1/6 then
				return p + (q - p) * 6 * t
			elseif t < 1/2 then
				return q
			elseif t < 2/3 then
				return p + (q - p) * (2/3 - t) * 6
			else
				return p
			end
		end

		if l < 0.5 then
			q = l * (1 + s)
		else
			q = l + s - l * s
		end
		p = 2 * l - q
		r = hue2rgb(h + 1/3)
		g = hue2rgb(h)
		b = hue2rgb(h - 1/3)
	end
	return r, g, b
end

local function rgb2hsl(r, g, b)
  local max = math.max(r, g, b)
  local min = math.min(r, g, b)
  local h,s, l = (max + min) / 2 -- luminance
  s, l = h, h
  if max == min then
    h,s = 0,0 -- achromatic
  else
    local d = max - min;
    if l > 0.5 then
      s = d / (2 - max - min)
    else
      s = d / (max + min)
    end
    if max == r then
      if g < b then
        h = (g - b) / d + 6
      else
        h = (g - b) / d + 0
      end
    elseif max == g then
      h = (b - r) / d + 2
    elseif max == b then
      h = (r - g) / d + 4
    end
    h = h / 6
  end
  return h, s, l
end

local function cmyk2rgb(c, m, y, k)
  local r, g, b
  r = 1.0 - (c * (1.0 - k) + k)
  g = 1.0 - (m * (1.0 - k) + k)
  b = 1.0 - (y * (1.0 - k) + k)
  return r, g, b
end

local function rgb2cmyk(r, g, b)
  local c, m, y, k
  if (r == 0) and (g == 0) and (b == 0) then
    c, m, y, k = 0, 0, 0, 1
  elseif (r == 1) and (g == 1) and (b == 1) then
    c, m, y, k = 0, 0, 0, 0
  else
    c = 1.0 - r
    m = 1.0 - g
    y = 1.0 - b
    local minK = math.min(r, g, b)
    c = (c - minK) / (1.0 - minK)
    m = (m - minK) / (1.0 - minK)
    y = (y - minK) / (1.0 - minK)
    k = minK
  end
  return c, y, m, k
end

local function floatTo8888(r, g, b, a)
	return floor(r*255), floor(g*255), floor(b*255), floor(a*255)
end

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

local events = {
	startup = function()
		local screen = surface.wrap(window.screen, false)
		-- desired count of blocks in both horizontal and vertical direction
		local blocksCountX, blocksCountY = 64, 64

		local delimiterSize = 1
		-- certain Window Managers tend to occupy a part of the application screen with controls or a title bar
		local windowBorderX = 0
		local windowBorderY = 0

		-- determine color block size
		local blockWidth = floor((window.width - windowBorderX)/(blocksCountX - 1 + delimiterSize))
		local blockHeight = floor((window.height - windowBorderY)/(blocksCountY - 1 + delimiterSize))

		for y = 0, blocksCountY-1 do
			for x = 0,blocksCountX-1 do
				local xPos = floor((blockWidth + delimiterSize) * x)
				local yPos = floor((blockHeight + delimiterSize) * y)

				local rectangle = {
					x = xPos,
					y = yPos,
					w = floor(blockWidth),
					h = floor(blockHeight),
				}

				-- cycle throgh all color hues
				local hue = x/(blocksCountX-1)
				-- saturation will graudally decrease
				local saturation = 1 - y/(blocksCountY-1)
				-- full color brightness uses lightness value 0.5
				local lightness = 0.5

				-- convert HSL color to RGB color model
				local r,g,b = hsl2rgb(hue, saturation, lightness)

				local color = screen.mapRGBA(floatTo8888(r, g, b, 1))
				-- draw a rectangle filled with specified color
				screen.fill(rectangle, color)
			end
		end

		-- flip content of the back-buffer into front-buffer (screen)
		SDL.SDL_Flip(window.screen)
	end,
}

app.main(window, events)

