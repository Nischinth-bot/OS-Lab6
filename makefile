CC = gcc
CFLAGS = -g -c -Wall
LDLIBS = -lpthread 
.c.o:
	$(CC) $(CFLAGS) $< -o $@

monitor: monitor.o fileHandle.o mainThread.o

mainThread.o : mainThread.h config.h fileHandle.h

fileHandler.o: fileHandle.h config.h

clean:
	rm *.o 
