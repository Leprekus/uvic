# Get all .c files in the current directory
SRCS = $(wildcard *.c)

# Replace the .c extension with .o
OBJS = $(SRCS:.c=.o)

# Compiler and flags definitions (Best Practice)
CC = gcc
CFLAGS = -Wall -Wextra -g

all: main

# Link object files into the final executable (Includes -g via CFLAGS)
main: $(OBJS)
	$(CC) $(CFLAGS) -o uvgz $(OBJS)
	
# Compile source files into object files (Includes -g via CFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f main $(OBJS)

.PHONY: all clean
