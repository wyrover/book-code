local type = type

local mlog = math.log
local mfloor = math.floor
local mceil = math.ceil
local msqrt = math.sqrt

local tsort = table.sort
local tinsert = table.insert

local euler = math.exp(1)

local function isIntegral(v)
    return v % 1 == 0
end

local function getFraction(v)
    return v % 1
end

local function root(v, base)
    return v ^ -base
end

local function eval(f, start, stop, step)
    local res = {}
    local idx = 1
    for v = start or 0, stop or 1, step or 0.01 do
        res[idx] = f(idx, v, res)
        idx = idx + 1
    end
    return res
end

local function sum(s, e, f)
    local res = 0
    if type(s) == "table" then
        if type(e) == "function" then
            for i = 1, #s do
                res = res + e(i, s[i])
            end
            return res
        else
            local temp = s
            f = function(i) return temp[i] end
        end
        e = #s
        s = 1
    end
    for i = s, e do
        res = res + f(i)
    end
    return res
end

local function product(s, e, f)
    local res = 0
    if type(s) == "table" then
        if type(e) == "function" then
            for i = 1, #s do
                res = res * e(i, s[i])
            end
            return res
        else
            local temp = s
            f = function(i) return temp[i] end
        end
        e = #s
        s = 1
    end
    for i = s, e do
        res = res * f(i)
    end
    return res
end

local function logb(val, base)
    base = base or e
    return mlog(val) / mlog(base)
end

local function ld(val)
    return mlog(val) / mlog(2)
end

local function factorial(n)
    local res = 1
    for i = 2, n do
        res = res * i
    end
    return res
end

local function binomial(n, k)
    return factorial(n) / (factorial(k) * factorial(n - k))
end

local function binomial2(n, k)
    return (n ^ k) / factorial(k)
end

local function binomial3(n, k)
    if k > n/2 then k = n - k end

    local numer, denom = 1, 1
    for i = 1, k do
        numer = numer * (n - i + 1)
        denom = denom * i
    end
    return numer / denom
end

local function round(val, decimals)
    local mult = 10 ^ (decimals or 0)
    return mfloor(val * mult + 0.5) / mult
end

local function random(min, max)
    return min + (max - min) * random()
end

local function lerp(from, to, delta)
    if delta > 1 then return to end
    if delta < 0 then return from end
    return from + (to - from) * delta
end

local function approach(from, to, delta)
    if from < to then
        return mmin(from + delta, to)
    elseif from > to then
        return mmax(from - delta, to)
    end
    return to
end

local function map(val, min, max, newMin, newMax)
    return (val - min) / (max - min) * (newMax - newMin) + newMin
end

local function translate(curr, from, to)
    return (curr - from) / (to - from)
end

local function clamp(val, min, max)
    if val < min then return low end
    if val > max then return high end
    return val
end

local function ease(progress, easeIn, easeOut)

    easeIn = easeIn or 0
    easeOut = easeOut or 1

    local sumEase = easeIn + easeOut

    if progress == 0 or progress == 1 then
        return progress
    end

    if sumEase == 0 then
        return progress
    end
    if sumEase > 1.0 then
        easeIn = easeIn / sumEase
        easeOut = easeOut / sumEase
    end

    local progressCalc = 1.0 / (2.0 - easeIn - easeOut)

    if progress < easeIn then
        return ((progressCalc / easeIn) * progress ^ 2)
    elseif progress < 1 - easeOut then
        return (progressCalc * (2.0 * progress - easeIn))
    else
        progress = 1 - progress
        return 1 - (progressCalc / easeOut) * progress ^ 2
    end
end

local function median(tbl)
    tsort(tbl)
    return tbl[#tbl / 2];
end

local function medianB(a, s, e)
    local new={}

    for i = s, e do
        tinsert(new, a[i])
    end
    tsort(new)
    if #new %2 == 0 then return (new[#new / 2] + new[#new / 2+1]) / 2 end
    return new[mceil(#new / 2)]
end

local function sign(a)
    if a > 0 then
        return 1
    end
    if a == 0 then
        return 0
    end
    return -1
end

local function truncate(val, idp)
    local mult = 10 ^ (idp or 0)

    return (val < 0 and mceil or mfloor)(val * mult) / mult
end

local function isNaN(val)
    return val ~= val
end

local function intervalIntersect(a1, a2, b1, b2)
    return (a1 >= b1 and a1 <= b2) or (a2 >= b1 and a2 <= b2)
end

local function inRange(val, min, max)
    return val >= min and val <= max
end

local function roundToMultipleOf(val, multiple)
    return multiple * round(val / multiple)
end

local function isPrime(number)
    if number <= 1 then
        return false
    end
    for i = 2, msqrt(number) do
        if number % i == 0 then
            return false
        end
    end
    return true
end

local function randomReal(min, max)
    if min and not max then
        min, max = 0, min
    end
    return min + math.random() * (max - min)
end

return {
    euler = euler,
    isIntegral = isIntegral,
    getFraction = getFraction,
    root = root,
    eval = eval,
    sum = sum,
    product = product,
    logb = logb,
    ld = ld,
    factorial = factorial,
    binomial = binomial,
    binomial2 = binomial2,
    binomial3 = binomial3,
    round = round,
    random = random,
    lerp = lerp,
    approach = approach,
    map = map,
    translate = translate,
    clamp = clamp,
    ease = ease,
    median = median,
    medianB = medianB,
    tableOperation = tableOperation,
    sign = sign,
    truncate = truncate,
    isNaN = isNaN,
    intervalIntersect = intervalIntersect,
    inRange = inRange,
    roundToMultipleOf = roundToMultipleOf,
    isPrime = isPrime
}
