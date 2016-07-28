.PHONY:clean
CC=gcc
CPPFLAGS=-unused-but-set-variable 
BIN= main
OBJS= ex-1.c list.c set.c graph.c
$(BIN):$(OBJS)
	$(CC) $(CPPFLAGS) $^ -o $@
%.o:%.c
	$(CC) $(CPPFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
