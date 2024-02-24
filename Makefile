OUT = build
SRC = src
INC = include

LIST_H = $(INC)/list.h

BIN = $(OUT)/test
OBJ = $(OUT)/test.o

CFLAGS = -Wall -I$(INC)
LIBS = 

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LIBS)

# LIST_H is a prerequisite because it's the only thing that really matters for
# this project and everything should be recompiled if it changes
$(OUT)/%.o: $(SRC)/%.c $(LIST_H)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN) $(OBJ)

run: $(BIN)
	./$(BIN)

.PHONY: clean run
