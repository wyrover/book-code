require "iconv"

local function Convert(text_in, code_in, code_out)
	local cd = iconv.open(code_in, code_out)
	assert(cd, "Failed to create a converter object.")
	local text_out, err = cd:iconv(text_in)

	if err == iconv.ERROR_INCOMPLETE then
		print("ERROR: Incomplete input.")
	elseif err == iconv.ERROR_INVALID then
		print("ERROR: Invalid input.")
	elseif err == iconv.ERROR_NO_MEMORY then
		print("ERROR: Failed to allocate memory.")
	elseif err == iconv.ERROR_UNKNOWN then
		print("ERROR: There was an unknown error.")
	end
	return text_out
end

local text_dos866 = "ќв  бва®Є  ў Є®¤Ёа®ўЄҐ OEM 866"
local text_koi8r = "ьФБ УФТПЛБ Ч ЛПДЙТПЧЛЕ KOI8-R"
local text_utf8 = "Р­С‚Р° СЃС‚СЂРѕРєР° РІ РєРѕРґРёСЂРѕРІРєРµ UTF-8"

print ("Вывод строк, перекодированных в WIN-1251:")
print( Convert(text_dos866, "cp866", "windows-1251") )
print( Convert(text_koi8r, "koi8-r", "windows-1251") )
print( Convert(text_utf8, "utf8", "windows-1251") )
