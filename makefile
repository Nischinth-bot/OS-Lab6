CC = gcc
CFLAGS = -g -c -Wall
LDLIBS = -lpthread 
.c.o:
	$(CC) $(CFLAGS) $< -o $@

monitor: monitor.o fileHandle.o 

fileHandler.o: fileHandle.h config.h

clean:
	rm *.o 
