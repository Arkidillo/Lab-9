# Ensure that these targest always run
.PHONY: test clean

# DO NOT CHANGE THESE FLAGS
CC=gcc
CFLAGS=-std=gnu99 -Wall -pedantic -Werror -ggdb -g

make:
	$(CC) $(CFLAGS) ArduinoCommunicator.c -o ArduinoCommunicator -lm
	./ArduinoCommunicator

clean:
	# delete all compiled files
	rm -f ArduinoCommunicator
