CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Iinclude
SRC = main.c $(wildcard utils/*.c) $(wildcard dsa/*.c)
OBJ = $(SRC:.c=.o)
TARGET = main

PDIR = profiled
POBJ = $(addprefix $(PDIR)/, $(OBJ))


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

profile: CFLAGS += -pg
profile: $(POBJ)
	$(CC) $(CFLAGS) $(POBJ) -o $(TARGET)
$(PDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
