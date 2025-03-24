CC ?= $(CROSS_COMPILE)gcc
CUR_PATH=$(dir $(abspath $(MAKEFILE_LIST)))
CFLAGS ?= -g -Wall 
#-fsanitize=address,leak
LDLIBS ?= -lm -lpthread
#-lfile_manipulate -lsig_func
SRC=$(wildcard $(CUR_PATH)/*.c)
OBJS=$(SRC:.c=.o)

.PHONY: all aesdsocket clean

.DEFAULT_GOAL: all

all: aesdsocket

aesdsocket: aesdsocket.o
#$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	$(RM) aesdsocket $(wildcard *.o) 
