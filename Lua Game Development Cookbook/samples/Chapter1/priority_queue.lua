local ti = table.insert
local tr = table.remove

-- removes element from table by its value
local tr2 = function(t, v)
    for i=1,#t do
        if t[i]==v then
            tr(t, i)
            break
        end
    end
end

local function pqueue()
    -- interface table
    local t = {}

    -- a set of elements
    local set = {}
    -- a set of priority bags with a elements
    local r_set = {}
    -- sorted list of priorities
    local keys = {}

    -- add element into storage, set its priority and sort keys
    --  k - element
    --  v - priority
    local function addKV(k, v)
        set[k] = v
        -- create a new list for this priority
        if not r_set[v] then
            ti(keys, v)
            table.sort(keys)
            local k0 = {k}
            r_set[v] = k0
            setmetatable(k0, {
                __mode = 'v'
            })
            -- add element into list for this priority
        else
            ti(r_set[v], k)
        end
    end

    -- remove element from storage and sort keys
    local remove = function(k)
        local v = set[k]
        local prioritySet = r_set[v]
        tr2(prioritySet, k)
        if #prioritySet < 1 then
            tr2(keys, v)
            r_set[v] = nil
            table.sort(keys)
            set[k] = nil
        end
    end; t.remove = remove

    -- returns an element with the lowest priority
    t.min = function()
        local priority = keys[1]
        if priority then
            return r_set[priority][1] or {}
        else
            return {}
        end
    end

    -- returns an element with the highest priority
    t.max = function()
        local priority = keys[#keys]
        if priority then
            return r_set[priority][1] or {}
        else
            return {}
        end
    end

    -- is this queue empty?
    t.empty = function()
        return #keys < 1
    end
    -- simple element iterator, retrieves elements with
    -- the highest priority first
    t.iterate = function()
        return function()
            if not t.empty() then
                local element = t.max()
                t.remove(element)
                return element
            end
        end
    end
    -- setup pqueue behavior
    setmetatable(t, {
        __index = set,
        __newindex = function(t, k, v)
            if not set[k] then
                -- new item
                addKV(k, v)
            else
                -- existing item
                remove(k)
                addKV(k, v)
            end
        end,
    })
    return t
end

local q1 = pqueue()

local elements = {
    ['Lorem'] = 20,
    ['ipsum'] = 100,
    ['dolor'] = 80,
    ['sit'] = 30,
    ['amet'] = 20,
}

-- Place a few elements into queue
for element, priority in pairs(elements)  do
    q1[element] = priority
end

-- Traverse all elements starting with the highest priority
while not q1.empty() do
    local element = q1.max()
    q1.remove(element)
    print(element)
end
