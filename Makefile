CC := gcc
CFLAGS := -Wall -Wextra -O3 -march=native -g
LIBS := -lreadline

SRC := main.c
OUT := Ash

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(LIBS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
