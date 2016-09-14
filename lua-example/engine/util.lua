local util = {}

function util.test(arg)
end

-- Decodes URL Query String Entities, e.g. '%20' becomes ' ' (space)
function util.decode(s)
    s = s:gsub('+', ' ')
    s = s:gsub('\n', '')
    s = s:gsub('%%(%x%x)', function(h) return string.char(tonumber(h, 16)) end)
    return s
end

-- Transforms query string, e.g. 'cmd=get&par=whatever' into a map '{cmd = "get", par = "whatever"}'
function util.parseurl(qs)
    local map = {}
    qs:gsub('([^&=]+)=([^&=]*)&?',
        function(key, val) map[decode(key)] = decode(val) end)
    return map
end

function printf(...)
    io.write(string.format(...))
end




return util
