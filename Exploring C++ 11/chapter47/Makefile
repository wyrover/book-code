DEBUG = -g -pg -fprofile-arcs -ftest-coverage
CXXFLAGS = -ansi -pedantic -Wall -std=c++11
TEST = test.cpp fixed.cpp
TESTOBJ = $(TEST:.cpp=.o)

all : runtest

test : $(TESTOBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(TESTOBJ)

runtest : cleantest test
	./test

clean : cleantest
	$(RM) -r $(TESTOBJ) test html

cleantest :
	$(RM) *.{gcov,gcda}

test.o : test.cpp fixed.hpp
fixed.o : fixed.cpp fixed.hpp ioflags.hpp

.PHONY : runtest clean cleantest
