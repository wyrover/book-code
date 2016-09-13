function C()
  print("C 1")
  print(1 + nil)
  print("C 2")
end

function B()
  print("B 1")
  C()
  print("B 2")
end

function A()
  print("A 1")
  B()
  print("A 2")
end

print("Main 1")
local Code, ErrStr = pcall(A)
print("Main 2", Code and "Success" or ErrStr)
