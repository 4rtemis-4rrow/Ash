# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O3 -march=native

# Libraries
LIBS := -lreadline

# Source files and output executable
SRC := main.c
OUT := Ash

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(LIBS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
