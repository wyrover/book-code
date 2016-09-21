local root_dir = __ROOT_PREFIX or '../../common'

local getOSModule = dofile(root_dir..'/lib/utils/getOS.lua')
local getOS = getOSModule.getOS
local M = {}

local paths = {
	'./{module}',
	'./lib/{module}',
	'./lib/external/{module}',
	'./lib/external/platform-specific/{platform}/{architecture}/{module}',
	'{root}/{module}',
	'{root}/lib/{module}',
	'{root}/lib/external/{module}',
	'{root}/lib/external/platform-specific/{platform}/{architecture}/{module}',
	'/usr/lib/lua/5.1/{module}',
	'/usr/lib/x86_64-linux-gnu/lua/5.1/{module}',
}

local module_paths = {
	'?.{extension}',
	'?/init.{extension}',
	'?/core.{extension}',
}

local extensions = {
	Windows = 'dll',
	Linux = 'so',
	MacOS = 'dylib',
}

local function pipe(cmd)
	local f = assert(io.popen(cmd))
	local output = f:read('*a')
	f:close()
	return output
end

local current_platform, current_architecture = getOS()

M.info = {
	platform = current_platform,	
	architecture = current_architecture,
}

local cpaths, lpaths = {}, {}
local current_clib_extension = extensions[current_platform]
if current_clib_extension then
	for _, path in ipairs(paths) do
		local path = path:gsub("{(%w+)}", {
			root = root_dir,
			platform = current_platform,
			architecture = current_architecture,
		})
		if #path>0 then
			for _, _module_path in ipairs(module_paths) do
				local module_path = path:gsub("{(%w+)}", {
					module = _module_path
				})
				cpaths[#cpaths+1] = module_path:gsub("{(%w+)}", {
					extension = current_clib_extension
				})
				lpaths[#lpaths+1] = module_path:gsub("{(%w+)}", {
					extension = 'lua'
				})
				lpaths[#lpaths+1] = module_path:gsub("{(%w+)}", {
					extension = 'luac'
				})
			end
		end
	end
	package.path = table.concat(lpaths, ";")
	package.cpath = table.concat(cpaths, ";")
else
	error('Current OS is not supported')
end

return M
