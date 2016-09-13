-- Chapter 2 has no file-based examples.  This file contains
-- most of the example code from the chapter.  Although the
-- whole file can be run, the best use is to cut-and-paste
-- the individual snippets into an interpreter session.

-- Assignment
NumberA = 2
print(NumberA)
NumberB = 2 + 2
print(NumberB)
print(NumberA + NumberB)

-- Multiple Assignment
Sum, Product, Exponent = 10 + 10, 10 * 10, 10 ^ 10
print(Sum, Product, Exponent)

A, B = 1, 2
print(A, B)
A, B = B, A
print(A, B)
John, Jane, Jolene = "chips", "lemonade", "egg salad"
print(John, Jane, Jolene)
John, Jane, Jolene = Jane, Jolene, John
print(John, Jane, Jolene)

-- String Quoting
print("This is a string!")
print('This is also a string.')
print('Cry "Havoc," and let slip the dogs of war')
print("Cry 'Havoc,' and let slip the dogs of war")

print([[There are some
funky "\' characters
in this string.]])
print([[+-----+
| Lua |
+-----+
]])
print([[
+-----+
| Lua |
+-----+
]])
print([=[Here ]] are some square brackets.]=])

-- Backslashes
print("Cry \"Havoc\"")
print('Ain\'t nobody\'s business if you do!')
print("1 backslash: \\ 2: \\\\ 3: \\\\\\")
print("one\
two")
print("one\ntwo")

-- Relational Operators and Boolean Values
print(4 < 5)
print(5 < 4)
Boolean1, Boolean2, Boolean3 = true, false, 1 < 2
print(Boolean1, Boolean2, Boolean3)
print(1 > 1)
print(1 > 2)
print(1 >= 1)
print(1 >= 2)
print(1 == 1)
print(1 == 2)
print(1 ~= 1)
print(1 ~= 2)

Str1 = "Jane X. Doe"
Str2 = "Jane X. Doe"
print(Str1 == Str2)
print("a" < "b")
print("a" < "a")
print("a" <= "a")
print("A" < "a")
print(" " > "\n")
print("abcd" < "abce")
print("" < "A")
print("" < "\0")
print("abc" < "abcd")

-- The Value nil
print(Asdf)

Password = "$xa(yb`y"
print(Password)
Password = nil
print(Password)

C, D = 3, 4
A, B, C, D = 1, 2
print(A, B, C, D)

A, B = 1, 2, 3, 4
print(A, B)

print(nil ~= false)
print(nil ~= 0)
print(nil ~= "nil")
print(nil == nil)

-- The and Operator
print(false and false)
print(false and true)
print(true and false)
print(true and true)

Num = 15
print(Num > 10 and Num < 20)

print(false and nil)
print(nil and false)
print(false and 1)
print(1 and false)
print(nil and 1)
print(1 and nil)
print(nil and true)
print(true and nil)
print(true and 1)
print(1 and true)
print("a" and 1)
print(1 and "a")

-- The or Operator
print(false or false)
print(false or true)
print(true or false)
print(true or true)

print(42 or nil)
print(nil or 42)
print(false or nil)
print(nil or false)
print("first" or "second")
print(false or "second")

print(FavoriteFruit or "apple")
FavoriteFruit = "kiwi"
print(FavoriteFruit or "apple")

Choice1, Choice2, Choice3 = nil, "pencil", "pen"
print(Choice1 or Choice2 or Choice3)

-- The not Operator
print(not false)
print(not nil)
print(not true)
print(not 50)
print(not "Hello")

print(not not true)
print(not not 50)
print(not not "Hello")
print(not not false)
print(not not nil)

-- The String Concatenation Operator
print("App" .. "le")
print("a" .. "b" .. "c" .. "d")
Name = "dear reader"
print("Hello, " .. Name .. ", how are you?")

print("Jane" .. "Doe")
print("Jane" .. " " .. "Doe")

OneAndTwoThree, OneTwoAndThree = "1" .. "23", "12" .. "3"
print(OneAndTwoThree, OneTwoAndThree)
print(OneAndTwoThree == OneTwoAndThree)
print("abcd" .. "" == "abcd")

-- The Length Operator
print(#"")
print(#"!")
print(#"\n")
print(#"abcd")
VerbForms = "see saw seen"
print(#VerbForms)

print(#"fiŝo")
NullByte = "\0"
NullBytes = NullByte .. NullByte .. NullByte
print(#NullByte)
print(#NullBytes)
print(#"before\0between\0behind")

-- The Modulo Operator
Hour = 3
Hour = Hour + 2
Hour = Hour % 12
print(Hour)
Hour = Hour + 144
Hour = Hour % 12
print(Hour)
Hour = Hour - 149
Hour = Hour % 12
print(Hour)

-- Automatic Conversion of Operands
print("2" + "2")
print(111 .. 999)

-- Precedence and Associativity
print(5 * 5 + 1)
print(1 + 5 * 5)
print(5 * (5 + 1))
print((1 + 5) * 5)

CreditA, DebitA, CreditB, DebitB = 1000, 150, 500, 25
print(CreditA - DebitA == CreditB - DebitB)
print(CreditA - DebitA > CreditB - DebitB)

CreditA, DebitA, CreditB, DebitB = 1000, 150, 500, 505
print(CreditA >= DebitA and CreditB >= DebitB)
print(CreditA >= DebitA or CreditB >= DebitB)

B, C = "B was chosen", "C was chosen"
A = true
print(A and B or C)
A = false
print(A and B or C)

B, C = false, "C was chosen"
A = true
print(A and B or C)

-- Comments
print("Hello") -- This is a comment.
print("Goodbye") -- So is this.

-- print("Hello")

--[[ This is
  a multi-line
  comment.]]


print(2 + --[[2 +]] 2)

--[[
print("Multiple")
print("lines.")
--]]

---[[
print("Multiple")
print("lines.")
--]]

-- The if Statement

-- Without if:
Str1, Str2 = "aardvark", "zebra"
print(Str1 < Str2 and Str1 .. Str2 or Str2 .. Str1)
Str1, Str2 = "zebra", "aardvark"
print(Str1 < Str2 and Str1 .. Str2 or Str2 .. Str1)

-- With if:
Str1, Str2 = "aardvark", "zebra"
if Str1 < Str2 then
  print(Str1 .. Str2)
else
  print(Str2 .. Str1)
end
-- Do it again with the strings swapped:
Str1, Str2 = "zebra", "aardvark"
if Str1 < Str2 then
  print(Str1 .. Str2)
else
  print(Str2 .. Str1)
end

-- No else clause:
if not Asdf then print("Asdf is false") end
-- No else clause:
if "X" .. "Y" .. "Z" == "XYZ" then
  Name = "Virginia"
  print("Yes, " .. Name .. ", it's \"XYZ\"")
end

-- Alphabetical concatenation with no else clause:
Str1, Str2 = "aardvark", "zebra"
if Str1 > Str2 then
  Str1, Str2 = Str2, Str1
end
print(Str1 .. Str2)
-- Do it again with the strings swapped:
Str1, Str2 = "zebra", "aardvark"
if Str1 > Str2 then
  Str1, Str2 = Str2, Str1
end
print(Str1 .. Str2)

if Str1 == Str2 then
  -- Str2 is a duplicate of Str1; replace it with the
  -- empty string:
  Str2 = ""
else
  if Str1 > Str2 then
    -- They're in the wrong order; swap 'em:
    Str1, Str2 = Str2, Str1
  end
end
print(Str1 .. Str2)

-- elseif clauses:
if Str1 == Str2 then
  -- Str2 is a duplicate of Str1; replace it with the
  -- empty string:
  Str2 = ""
elseif Str1 > Str2 then
  -- They're in the wrong order; swap 'em:
  Str1, Str2 = Str2, Str1
end
print(Str1 .. Str2)

-- Multiple elseif clauses:
if N == 1 then
  print("N is one")
elseif N == 2 then
  print("N is two")
elseif N == 3 then
  print("N is three")
else
  print("N is neither one nor two nor three")
end

-- The while Loop
while false do
  print("This will")
  print("never print.")
end

--[[ This runs forever (if not commented out):
while true do
  print("It keeps")
  print("going and going.")
end
--]]

-- Count to 10:
Counter = 1
while Counter <= 10 do
  print(Counter)
  Counter = Counter + 1
end

-- Factorials less than 100:
N, F = 1, 1
while F < 100 do
  print(N .. "! is " .. F)
  N = N + 1
  -- Compute the factorial of the new N based on
  -- the factorial of the old N:
  F = F * N
end

-- The for Loop

-- Count to 10:
for Counter = 1, 10 do
  print(Counter)
end

for N = 1, 0 do
  print(N)
end
for N = 3, 1, -1 do
  print(N)
end
for N = -50, -45.25, 2.5 do
  print(N)
end
for N = 2, 5, 2 do
  print(N)
end

End = 5
for I = 1, End do
  End = End - 1 -- The loop doesn't care that End is being
    -- changed.
  print("I is " .. I .. " and End is " .. End)
end

-- Nested loops:
for Outer = 1, 3 do
  for Inner = 1, 3 do
    print("Outer: " .. Outer .. "; Inner: " .. Inner)
  end
end

-- Two different N's:
N = "outside"
for N = 1, 3 do
  print(N)
end
print(N)

-- The repeat Loop
N, F = 1, 1
repeat
  print(N .. "! is " .. F)
  -- Compute the next N and its factorial:
  N = N + 1
  F = F * N
until F >= 100

-- The break and do Statements
for N = 1, 10 do
  if N > 5 then
    break
  end
  print(N)
end

for Outer = 1, 3 do
  for Inner = 101, 103 do
    print("Outer: " .. Outer .. "; Inner: " .. Inner)
    if Inner == 102 then
      print("Breaking out of inner loop; 103 won't be reached.")
      break
    end
  end
end

for N = 1, 10 do
  print("Before")
  do break end
  print("After")
end

do
  print("\nHere are some numbers:")
  for N = 1, 5 do
    print(N)
  end
  print("There were some numbers.")
end
