OUT = build
SRC = src

LIST_H = $(SRC)/list.h

EXAMPLE_BIN = $(OUT)/example
EXAMPLE_OBJ = $(OUT)/example.o

TEST_BIN = $(OUT)/test
TEST_OBJ = $(OUT)/test.o \
	   $(OUT)/avlist.o

CFLAGS = -g -Wall
LIBS = 

all: $(EXAMPLE_BIN) $(TEST_BIN)

$(EXAMPLE_BIN): $(EXAMPLE_OBJ)
	$(CC) -g -o $(EXAMPLE_BIN) $(EXAMPLE_OBJ) $(LIBS)

$(TEST_BIN): $(TEST_OBJ)
	$(CC) -g -o $(TEST_BIN) $(TEST_OBJ) $(LIBS)

# LIST_H is a prerequisite because it's the only thing that really matters for
# this project and everything should be recompiled if it changes
$(OUT)/%.o: $(SRC)/%.c $(LIST_H)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXAMPLE_BIN) $(TEST_BIN) $(EXAMPLE_OBJ) $(TEST_OBJ)

example: $(EXAMPLE_BIN)
	./$(EXAMPLE_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

.PHONY: all clean example test
