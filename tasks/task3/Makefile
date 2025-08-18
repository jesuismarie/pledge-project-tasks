CC      := cc
CFLAGS  := -Wall -Wextra -Wpedantic -Werror
LDFLAGS := -lm -lpthread
IFLAGS := -Iinclude -DPROJECT_NAME=\"DemoApp\"

SRC := src/main.c
OBJ := $(SRC:.c=.o)
BIN := app

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
