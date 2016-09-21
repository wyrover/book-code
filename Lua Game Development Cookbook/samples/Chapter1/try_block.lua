local function f1(a, b)
  assert((a == 1), "The first parameter must be equal to 1")
  print(b)
  return a+1
end

function try(fn, catch_fn)
  local status, msg = pcall(fn)
  if not status then
	  catch_fn(msg)
  end
end

try(function()
  f1(2, 3) -- this will throw "an exception"
end, function(e)
  print('An exception occured:', e)
  error('Throw exception')
end)
