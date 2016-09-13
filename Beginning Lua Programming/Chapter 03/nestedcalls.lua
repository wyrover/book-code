-- A demonstration of functions calling functions.

function A()
  print("  About to enter B")
  B()
  print("  Just exited B")
end

function B()
  print("    About to enter C")
  C()
  print("    Just exited C")
end

function C()
  --print(nil + nil) -- Uncomment this for an error.
  print("      Inside C")
end

print("About to enter A")
A()
print("Just exited A")
