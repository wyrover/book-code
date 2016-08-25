# Script for testing all examples programs with the GNU C++ Compiler
set Compile = "g++ -I."

echo " "
echo "Testing ex01-01.cpp"
$Compile  ex01-01.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex01-02.cpp"
$Compile  ex01-02.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex01-03.cpp"
$Compile  ex01-03.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex01-04.cpp"
$Compile  ex01-04.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-01.cpp"
$Compile  ex02-01.cpp
if ($status == 0) then
  a.out 
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-02.cpp"
$Compile  ex02-02.cpp
if ($status == 0) then
  a.out  <ex02-02.inp
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-03.cpp"
$Compile  ex02-03.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-04.cpp"
$Compile  ex02-04.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-05.cpp"
$Compile  ex02-05.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-06.cpp"
$Compile  ex02-06.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-07.cpp"
$Compile  ex02-07.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-08.cpp"
$Compile  ex02-08.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-09.cpp"
$Compile  ex02-09.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-10.cpp"
$Compile  ex02-10.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-11.cpp"
$Compile  ex02-11.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-12.cpp"
$Compile  ex02-12.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex02-13.cpp"
$Compile  ex02-13.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex04-01.cpp"
$Compile  ex04-01.cpp
if ($status == 0) then
  a.out <ex04-01.inp
  /usr/bin/rm a.out 
endif

echo " "
echo "Testing ex05-01.cpp"
$Compile  ex05-01.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-02.cpp"
$Compile  ex05-02.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-03.cpp random.cpp"
$Compile  ex05-03.cpp random.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-04.cpp"
$Compile  ex05-04.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-05.cpp"
$Compile  ex05-05.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-06.cpp"
$Compile  ex05-06.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-07.cpp"
$Compile  ex05-07.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-08.cpp"
$Compile  ex05-08.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-09.cpp"
$Compile  ex05-09.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-10.cpp"
$Compile  ex05-10.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-11.cpp"
$Compile  ex05-11.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-12.cpp"
$Compile  ex05-12.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-13.cpp tempbuf.cpp"
$Compile  ex05-13.cpp tempbuf.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-14.cpp random.cpp"
$Compile  ex05-14.cpp random.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-15.cpp"
$Compile  ex05-15.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-16.cpp"
$Compile  ex05-16.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-17.cpp"
$Compile  ex05-17.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-18.cpp"
$Compile  ex05-18.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-19.cpp"
$Compile  ex05-19.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-20.cpp"
$Compile  ex05-20.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-21.cpp"
$Compile  ex05-21.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-22.cpp tempbuf.cpp"
$Compile  ex05-22.cpp tempbuf.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-23.cpp"
$Compile  ex05-23.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-24.cpp"
$Compile  ex05-24.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-25.cpp tempbuf.cpp"
$Compile  ex05-25.cpp tempbuf.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-26.cpp"
$Compile  ex05-26.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-27.cpp random.cpp"
$Compile  ex05-27.cpp random.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-28.cpp random.cpp"
$Compile  ex05-28.cpp random.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-29.cpp"
$Compile  ex05-29.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-30.cpp"
$Compile  ex05-30.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-31.cpp"
$Compile  ex05-31.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-32.cpp"
$Compile  ex05-32.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-33.cpp"
$Compile  ex05-33.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex05-34.cpp"
$Compile  ex05-34.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-01.cpp"
$Compile  ex06-01.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-02.cpp"
$Compile  ex06-02.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-03.cpp"
$Compile  ex06-03.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-04.cpp"
$Compile  ex06-04.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-05.cpp"
$Compile  ex06-05.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-06.cpp"
$Compile  ex06-06.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-07.cpp"
$Compile  ex06-07.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-07a.cpp"
$Compile  ex06-07a.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-08.cpp"
$Compile  ex06-08.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-09.cpp"
$Compile  ex06-09.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-10.cpp"
$Compile  ex06-10.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-11.cpp"
$Compile  ex06-11.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-12.cpp"
$Compile  ex06-12.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex06-13.cpp"
$Compile  ex06-13.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex07-01.cpp"
$Compile  ex07-01.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex07-02.cpp"
$Compile  ex07-02.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex07-03.cpp"
$Compile  ex07-03.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex07-03a.cpp"
$Compile  ex07-03a.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex07-04.cpp"
$Compile  ex07-04.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex07-04a.cpp"
$Compile  ex07-04a.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex07-05.cpp"
$Compile  ex07-05.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex07-06.cpp"
$Compile  ex07-06.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex12-01.cpp"
$Compile  ex12-01.cpp
if ($status == 0) then
  a.out <dictname
  /usr/bin/rm a.out 
endif

echo " "
echo "Testing ex13-01.cpp"
$Compile  ex13-01.cpp
if ($status == 0) then
  a.out <dictname
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex14-01.cpp"
$Compile  ex14-01.cpp
if ($status == 0) then
  a.out  <dictname
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex15-01.cpp"
$Compile  ex15-01.cpp
if ($status == 0) then
  a.out <dictname
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex16-01.cpp"
$Compile  ex16-01.cpp
if ($status == 0) then
  a.out
  /usr/bin/rm a.out
endif

echo " "
echo "Testing ex17-01.cpp shape.o screen.o"
$Compile  -c shape.cpp 
$Compile  -c screen.cpp 
$Compile  ex17-01.cpp shape.o screen.o
if ($status == 0) then
  a.out >ex17-01.out
  /usr/bin/rm shape.o
  /usr/bin/rm screen.o
  /usr/bin/rm a.out
endif

