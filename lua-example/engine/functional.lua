local tinsert = table.insert

local function nest(func, n)
    if n == 1 then
        return func
    end
    return nest(function(...)
        return func(func(...))
    end, n - 1)
end

local function nestList(func, n, list)
    list = list or {}
    tinsert(list, func)
    if n == 1 then
        return func
    end
    nestList(function(...)
        return func(func(...))
    end, n - 1, list)
    return list
end

local function combineFlat(tbl)
    return function(...)
        for i = 1, #tbl do
            tbl[i](...)
        end
    end
end

return {
    nest = nest,
    nestList = nestList,
    combineFlat = combineFlat
}
