CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -std=c11 -Iinclude -Isrc -g --coverage
AR       = ar
ARFLAGS  = rcs

SRC_DIR  = src
INC_DIR  = include
TEST_DIR = test
LIB_DIR  = lib

SRC      = $(SRC_DIR)/tree.c $(SRC_DIR)/heap.c
OBJ      = $(SRC:.c=.o)

TEST_SRC = $(TEST_DIR)/test.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_BIN = $(TEST_DIR)/run

LIB      = $(LIB_DIR)/lib.a

CHECK_LIBS = -lcheck -lpthread -lm

.PHONY: all lib test clean gcov_report leaks dirs

all: dirs $(LIB) $(TEST_BIN)

lib: dirs $(LIB)

dirs:
	mkdir -p $(LIB_DIR)
	mkdir -p $(TEST_DIR)

$(LIB): $(OBJ) | dirs
	$(AR) $(ARFLAGS) $@ $^

$(TEST_BIN): $(LIB) $(TEST_OBJ) | dirs
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJ) $(LIB) $(CHECK_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: clean all
	./$(TEST_BIN)
	@gcov -b -c $(SRC) $(TEST_SRC)

leaks: $(TEST_BIN)
	leaks --atExit -- ./$(TEST_BIN)

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TEST_BIN) $(LIB)
	rm -f *.gcda *.gcno *.gcov
	rm -f $(SRC_DIR)/*.gcda $(SRC_DIR)/*.gcno $(SRC_DIR)/*.gcov
	rm -f $(TEST_DIR)/*.gcda $(TEST_DIR)/*.gcno $(TEST_DIR)/*.gcov
