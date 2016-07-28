.PHONY:clean
CC=gcc
CFLAGS=-Wall -g
BIN= main
OBJS= ex-2.c cover.c list.c set.c
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)

