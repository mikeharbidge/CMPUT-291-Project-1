# Makefile for CMPUT 291 Project 1
# To add your file to the compiling list add it to the SRCS line, right now only main

sorting: main.o
	gcc -Wall -std=c99 -L/usr/lib/sqlite3 -lsqlite3 main.o -o project1

main.o: main.c main.h
	gcc -Wall -std=c99 -L/usr/lib/sqlite3 -c main.c -lsqlite3

CC = gcc

#compiler flags:
CFLAGS = -std=c99 -Wall -L/usr/lib/sqlite3 -lsqlite3

SRCS = main.c
OBJS = $(patsubst %.c,%.o, $(SRCS)) 	#makes object files from all C files
HEADERS = $(wildcard *.h)

all: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): %.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	@rm -vf $(OBJS) $(OBJS2) sorting1 sorting2
