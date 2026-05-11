files=$(wildcard *.c)
# replace .c extension with .0
objects=$(files:.c=.o)

all: main

main: $(objects)
	gcc -o main $(objects)
	
# turn *.c into *.o
%.o: %.c
	gcc -c -g $< -o $@

clean:
	rm -f main $(objects)
