.PHONY:clean
CC=gcc
CPPFLAGS=-Wall
BIN= main
OBJS= ex-1.c heap.c
$(BIN):$(OBJS)
	$(CC) $(CPPFLAGS) $^ -o $@
%.o:%.c
	$(CC) $(CPPFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
