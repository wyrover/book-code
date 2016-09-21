local function queue()
    local out = {}
    local first, last = 0, -1
    out.push = function(item)
        last = last + 1
        out[last] = item
    end
    out.pop = function()
        if first <= last then
            local value = out[first]
            out[first] = nil
            first = first + 1
            return value
        end
    end
    out.iterator = function()
        return function()
            return out.pop()
        end
    end
    setmetatable(out, {
        __len = function()
            return (last-first+1)
        end,
    })
    return out
end

local q1 = queue()
-- Place a few elements into queue
for _, element in ipairs {'Lorem','ipsum','dolor','sit','amet'} do
    q1.push(element)
end

-- You can use iterator to process all elements in single for loop
for element in q1.iterator() do
    -- each queue element will be printed onto screen
    print(element)
end
