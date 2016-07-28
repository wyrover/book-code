CXXFLAGS = -ansi -pedantic -Wall -std=c++11

all: bmi

bmi: bmi.o
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) bmi.o

clean:
	$(RM) -r bmi bmi.o html

.PHONY: all clean
