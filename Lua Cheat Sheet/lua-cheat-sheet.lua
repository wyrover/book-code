print(package.cpath)
print(package.path)

-- Lua Cheat Sheet for Programmers, by Al Sweigart http://coffeeghost.net
-- This cheat sheet is an executable Lua program.
--[[ This is
a multline comment]]

---[[ This is a neat trick. The first -- makes -[[ not a multiline comment.
print("This line executes.")
--]] The rest of this line is also a comment.

print("Here is a string" .. ' concatenated with ' .. 2 .. ' other strings.')

-- Note: All number types are doubles. There're no integers.
print(type(42), type(42.0)) -- prints out "number  number"
variable_one = 1 + 2 - 3 -- This will equal zero.
variable_One = "Variables are case sensitive."
negative_twofiftysix = -2^8

a, b = 42, 101 --mulitple assignment
a, b = b, a    --provides a nice value swap trick
x, y, z = 1, 2, 3, "this value is discarded"

print(previously_unused_variable == nil) -- prints true, all vars start as nil
print(nil == 0 or nil == "") -- prints false, nil is not the same as false or 0
print('The # len operator says there are ' .. #'hello' .. ' letters in "hello".')

some_bool_variable = true and false or true and not false

a_table = {['spam'] = "Type something in:", ['eggs'] = 10} -- tables are dictionaries/arrays
print(a_table['spam'])

what_the_user_typed_in = io.read()
print('You typed in ' .. what_the_user_typed_in)

if 10 < 20 then
	print( "apple" == "orange" ) -- prints false
	print( "apple" ~= "orange" ) -- true, an apple is not equal to an orange
    local foo
    foo = 42
    print(foo)
elseif 50 < 100 then
	--These clauses can contain no lines of code.
end
print(foo) -- prints nil, local foo exists only in that "if" block above

m = 0
while m < 10 do
	print("howdy " .. m)
	m = m + 1 -- there is no m++ or m += 1
    repeat
        print("Repeat loops check the condition at end, and stops if it is true.")
        break -- breaks out of the loop early
    until m == 9999
end

for i = 1, 10 do
    for j = 1, 10, 2 do
        print("for loops add 1 to i and 2 to j each iteration " .. i .. ' ' .. j)
    end
end

function Greet(name)
	print('Hello ' .. name)
	bar = 100
	return "returns nil if you don't have a return statement."
end
Greet('Al Sweigart')
print(bar) -- prints 100



function serialize(obj)
    local lua = ""
    local t = type(obj)
    if t == "number" then
        lua = lua .. obj
    elseif t == "boolean" then
        lua = lua .. tostring(obj)
    elseif t == "string" then
        lua = lua .. string.format("%q", obj)
    elseif t == "table" then
        lua = lua .. "{\n"
    for k, v in pairs(obj) do
        lua = lua .. "[" .. serialize(k) .. "]=" .. serialize(v) .. ",\n"
    end
    local metatable = getmetatable(obj)
        if metatable ~= nil and type(metatable.__index) == "table" then
        for k, v in pairs(metatable.__index) do
            lua = lua .. "[" .. serialize(k) .. "]=" .. serialize(v) .. ",\n"
        end
    end
        lua = lua .. "}"
    elseif t == "nil" then
        return nil
    else
        error("can not serialize a " .. t .. " type.")
    end
    return lua
end

function unserialize(lua)
    local t = type(lua)
    if t == "nil" or lua == "" then
        return nil
    elseif t == "number" or t == "string" or t == "boolean" then
        lua = tostring(lua)
    else
        error("can not unserialize a " .. t .. " type.")
    end
    lua = "return " .. lua
    local func = loadstring(lua)
    if func == nil then
        return nil
    end
    return func()
end


data = {["a"] = "a", ["b"] = "b", [1] = 1, [2] = 2, ["t"] = {1, 2, 3}}
local sz = serialize(data)
print(sz)
print("---------")
print(serialize(unserialize(sz)))
