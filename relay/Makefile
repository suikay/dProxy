# relay Makefile
#

OBJ = 
RELAYOBJ = relay.o

CC = gcc
CFLAGS = -g 

all: relay 

relay: $(RELAYOBJ)
	$(CC) -o relay $(RELAYOBJ)

clean:
	rm -f relay $(RELAYOBJ)

dep:
	$(CC) -MM *.c -I ../include 

