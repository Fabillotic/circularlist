OUT = build
SRC = src
INC = include

BIN = $(OUT)/test
OBJ = $(OUT)/test.o

CFLAGS = -Wall -I$(INC)
LIBS = 

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LIBS)

$(OUT)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN) $(OBJ)

run: $(BIN)
	./$(BIN)

.PHONY: clean run
