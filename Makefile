CC = gcc
CFLAGS = -g -I/usr/include/plplot

# Linker flags
LDFLAGS = -L/usr/lib -lplplot -lm -lcjson

OBJDIR = obj
SRCDIR = $(shell pwd)
EXECDIR = exec

SRCS = $(wildcard *.c)
PRE_OBJS = $(SRCS:.c=.o)
#OBJS = $(SRCS:.c=.o)


OBJS := $(addprefix $(OBJDIR)/, $(PRE_OBJS))
engine: ${PRE_OBJS}
	$(CC) $(LDFLAGS)  ${OBJS} -o $@  
	cp engine a.out

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o ${OBJDIR}/$@

clean:
	rm engine $(OBJS)
