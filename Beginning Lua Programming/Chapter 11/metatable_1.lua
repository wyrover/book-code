MtTbl = {__eq = function(T1, T2) return T1.Val == T2.Val end}
A = {Val = 10}
setmetatable(A, MtTbl)
B = {Val = 10}
setmetatable(B, MtTbl)
C = {Val = 12}
setmetatable(C, MtTbl)
print(MtTbl, getmetatable(A), getmetatable(B), getmetatable(C))
print("A == B", A == B)
print("A == C", A == C)
print("rawequal(A, B)", rawequal(A, B))
print("rawequal(A, C)", rawequal(A, C))
