# Makefile for CMPUT 291 Project 1
# To add your file to the compiling list add it to the SRCS line
CC = gcc

#compiler flags:
CFLAGS = -std=c99 -Wall -L/usr/lib/sqlite3 

SRCS = main.c Login.c
OBJS = $(patsubst %.c,%.o, $(SRCS)) 	#makes object files from all C files
HEADERS = $(wildcard *.h)

project: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lsqlite3

$(OBJS): %.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $< -lsqlite3

.PHONY: clean
clean:
	@rm -vf $(OBJS) project
