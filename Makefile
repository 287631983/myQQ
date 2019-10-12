OBJS=client server
CFLAGS=-g -Wall -o0
.PHONY:clean
all:$(OBJS)
%:%.c
	@$(CC) $(CFLAGS) $^ -o $@ -ljson -lsqlite3
clean:
	@$(RM) $(OBJS)
	
