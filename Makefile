CC = gcc
CFLAGS = -Wall -std=c99
OUTFILE = randmst
OBJS = randmst.o
SRCS = randmst.c
LIBS = -lm

$(OUTFILE): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTFILE) $(OBJS) $(LIBS)
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)