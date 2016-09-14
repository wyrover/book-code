local format = {}

local sizeUnits = { "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB" }

function format.fileSizeFormat (fileSize)
	local unitIndex = 1
	while fileSize >= 1024 and sizeUnits [unitIndex + 1] do
		fileSize = fileSize / 1024
		unitIndex = unitIndex + 1
	end
	
	if fileSize == math.floor (fileSize) then
		return string.format ("%d %s", fileSize, sizeUnits [unitIndex])
	else
		return string.format ("%.2f %s", fileSize, sizeUnits [unitIndex])
	end
end

return format