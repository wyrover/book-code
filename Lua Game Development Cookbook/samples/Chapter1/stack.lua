local function stack()
    local out = {}
    out.push = function(item)
        out[#out+1] = item
    end
    out.pop = function()
        if #out>0 then
            return table.remove(out, #out)
        end
    end
    out.iterator = function()
        return function()
            return out.pop()
        end
    end
    return out
end

local s1 = stack()
-- Place a few elements into stack
for _, element in ipairs {'Lorem','ipsum','dolor','sit','amet'} do
    s1.push(element)
end

-- iterator function can be used to pop and process all elements
for element in s1.iterator() do
    print(element)
end
