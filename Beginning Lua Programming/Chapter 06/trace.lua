function B()
  print(debug.traceback("B"))
end

function A()
  print(debug.traceback("A 1"))
  B()
  print(debug.traceback("A 2"))
end

A()
