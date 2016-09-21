-- Window event handlers
local eventHandlers = {}

local function handleEvent(name, ...)
	local handlers = eventHandlers[name]
	if type(handlers)=="table" then
		for _, handler in ipairs(handlers) do
			if type(handler)=="function" then
				handler(...)
			end
		end
	end
end

local function addHandler(name, fn)
	local handlers = eventHandlers[name] or {}
	table.insert(handlers, fn)
	eventHandlers[name] = handlers
end

return {
	add = addHandler,
	invoke = handleEvent,
}
