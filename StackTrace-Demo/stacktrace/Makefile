CC = gcc

OBJS = trace.o symbols.o stack.o FileInject.o

stacktrace: $(OBJS)
	gcc -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

install: stacktrace
	cp stacktrace /usr/local/bin

clean:
	rm -f stacktrace *.o
