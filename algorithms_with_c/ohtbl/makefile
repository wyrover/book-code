.PHONY:clean
CC=gcc
CFLAGS=-Wall -g
BIN= main
OBJS= ex-1.c ohtbl.c
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)

