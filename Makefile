CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC = src/lrp.c main.c
OBJ = $(SRC:.c=.o)

TARGET = run


all: $(TARGET)


$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET)


.PHONY: all clean