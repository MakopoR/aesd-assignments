CROSS_COMPILE?=

default: all

all: writer

writer: writer.c
ifndef CROSS_COMPILE
	cc -o writer writer.c
else
ifeq ($(CROSS_COMPILE), aarch64-none-linux-gnu-)
	aarch64-none-linux-gnu-gcc -o writer writer.c
endif
endif
clean:
	rm -f writer
	rm -f $(wildcard *.o)
