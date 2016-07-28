# Makefile for the calculator project

CPPFLAGS = -MMD
CXXFLAGS = -O0 -g3 -Wall -std=c++11

SRC = calc_error.cpp node.cpp node_impl.cpp parse.cpp variables.cpp
OBJ = $(SRC:.cpp=.o)

TESTSRC=\
  test_main.cpp\
  test_node.cpp\
  test_parse.cpp
TESTOBJ = $(TESTSRC:.cpp=.o)

all : calc tester
	./tester

calc : calc.o $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

tester: $(OBJ) $(TESTOBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(RM) calc tester *.o *.d

-include *.d
