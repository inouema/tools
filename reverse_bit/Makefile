CC = gcc
CFLAGS = -O -Wall -W -I./
LDFLAGS =
LIBS = 
OBJS = main.o reverse_bit.o
PROGRAM = a.out 

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

.PHONY: clean
clean:
	rm -f *.o *.out
