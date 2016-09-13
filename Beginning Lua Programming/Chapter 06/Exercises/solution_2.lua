debug._traceback = debug.traceback

debug.traceback = function(Str)
  return debug._traceback('Programmer error: ' .. Str)
end
