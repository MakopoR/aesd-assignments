CC ?= $(CROSS_COMPILE)gcc
CUR_PATH=$(dir $(abspath $(MAKEFILE_LIST)))
CFLAGS ?= -g -Wall 
#-fsanitize=address,leak
LDFLAGS ?= -lm -lpthread
#-lfile_manipulate -lsig_func
SRC=$(wildcard $(CUR_PATH)/*.c)
OBJS=$(SRC:.c=.o)
TARGET ?= aesdsocket

.PHONY: all aesdsocket clean

.DEFAULT_GOAL: all

all: $(TARGET)

aesdsocket: $(TARGET).o
#$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	$(RM) aesdsocket $(wildcard *.o) 
