local sqlbuilder = {}
sqlbuilder._VERSION = "sql-builder 0.1"

sqlbuilder.select = function(what, tbl)
    local ret = {}
    local options = ""
    local prefix = "WHERE"
    local limit

    local query = string.format("SELECT %s FROM `%s`", escape.sql(what), escape.sql(tbl))

    local function generate_op(op)
        local op = op
        return function(key, value)
            if type(value) == "string" then
                value = string.format([["%s"]], escape.sql(value))
            else
                value = escape.sql(tostring(value))
            end
            key = escape.sql(tostring(key))

            query = string.format("%s %s %s %s %s", query, prefix, key, op, value)
            prefix = "AND"

            return ret
        end
    end

    ret.more_than = generate_op(">")
    ret.less_than = generate_op("<")
    ret.equals = generate_op("=")
    ret.less_than_or_equal = generate_op("<=")
    ret.more_than_or_equal = generate_op(">=")

    ret.limit = function(number)
        limit = number
        return ret
    end

    ret.get = function()
        if limit ~= nil then query = string.format("%s LIMIT %d", query, limit) end
        return query
    end

    return ret
end

return sqlbuilder

--[[
-- concept:
local user = req:params().user
local pass = hash_password(req:params().pass)
sqlbuilder.fromtable({
	SELECT = "*",
	FROM = "tbl",
	WHERE = {
		{
			{"user", "=", user}
			"AND",
			{"pass", "=", pass}
		},
		"OR",
		{
			{"user", "=", user},
			"AND",
			{"pass", "=", ""} -- idk, just to show the thingy
		}
	},
	LIMIT = 1
})
-- yields:
-- SELECT * FROM `tbl` WHERE (user="Kobra" AND pass="a94...f2e") OR (user="Kobra" AND pass="") LIMIT 1
]]
