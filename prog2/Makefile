CC = gcc
CFLAGS = -Wall -std=c99
OUTFILE = strassen
OBJS = strassen.o
SRCS = strassen.c
LIBS = -lm

$(OUTFILE): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTFILE) $(OBJS) $(LIBS)
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)