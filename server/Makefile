CROSS_COMPILE?=
FLAGS= -g -Wall -Werror

default: all

all: aesdsocket

aesdsocket: aesdsocket.c
ifndef CROSS_COMPILE
	cc -o aesdsocket aesdsocket.c $(CFLAGS) 
else
ifeq ($(CROSS_COMPILE), aarch64-none-linux-gnu-)
	aarch64-none-linux-gnu-gcc -o aesdsocket aesdsocket.c
endif
endif
clean:
	rm -f aesdsocket
	rm -f $(wildcard *.o)
