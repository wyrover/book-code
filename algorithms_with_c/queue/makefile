.PHONY:clean
CC=g++
CPPFLAGS=-g
BIN= main
OBJS=ex-1.o queue.o list.o
$(BIN):$(OBJS)
	$(CC) $(CPPFLAGS) $^ -o $@
%.o:%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)

