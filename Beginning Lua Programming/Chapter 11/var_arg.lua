function Test(...)
  io.write("*** Entering Test ***\n")
  for J = 1, select("#", ...) do
    io.write(select(J, ...))
    io.write("\n")
  end
end

List = {"One", "Two", "Three"}
Test(unpack(List))
Test(unpack(List, 1, 2))
Test(unpack(List, 2))
