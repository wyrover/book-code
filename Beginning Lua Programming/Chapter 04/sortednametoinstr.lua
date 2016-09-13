-- A demonstration of sorting an associative table.

NameToInstr = {John = "rhythm guitar",
  Paul = "bass guitar",
  George = "lead guitar",
  Ringo = "drumkit"}
-- Transfer the associative table NameToInstr to the
-- array Sorted:
Sorted = {}
for Name, Instr in pairs(NameToInstr) do
  table.insert(Sorted, {Name = Name, Instr = Instr})
end
-- The comparison function sorts by Name:
table.sort(Sorted, function(A, B) return A.Name < B.Name end)
-- Output:
for _, NameInstr in ipairs(Sorted) do
  print(NameInstr.Name .. " played " .. NameInstr.Instr)
end
