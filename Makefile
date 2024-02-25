OUT = build
SRC = src

LIST_H = $(SRC)/list.h

TESTBIN = $(OUT)/test
OBJ = $(OUT)/test.o

CFLAGS = -g -Wall
LIBS = 

$(TESTBIN): $(OBJ)
	$(CC) -g -o $(TESTBIN) $(OBJ) $(LIBS)

# LIST_H is a prerequisite because it's the only thing that really matters for
# this project and everything should be recompiled if it changes
$(OUT)/%.o: $(SRC)/%.c $(LIST_H)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TESTBIN) $(OBJ)

test: $(TESTBIN)
	./$(TESTBIN)

.PHONY: clean test
