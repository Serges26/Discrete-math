CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2 -g
GCOVFLAGS = -fprofile-arcs -ftest-coverage

INC_DIR = include
SRC_DIR = src
TEST_DIR = test
LIB_DIR = lib

LIB = $(LIB_DIR)/libgraph.a
TEST_BIN = $(TEST_DIR)/run

SRC = $(SRC_DIR)/graph.c
OBJ = $(SRC:.c=.o)

INTERNAL_HDR = $(SRC_DIR)/internal.h
PUBLIC_HDR = $(INC_DIR)/graph.h

LDLIBS = -lcheck -lm -lpthread


.PHONY: all clean test gcov_report leaks

all: test

$(LIB): $(OBJ) | $(LIB_DIR)
	ar rcs $@ $^

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(PUBLIC_HDR) $(INTERNAL_HDR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@


$(TEST_BIN): $(TEST_DIR)/test.c $(LIB)
	$(CC) $(CFLAGS) -I$(INC_DIR) $(TEST_DIR)/test.c $(LIB) -o $(TEST_BIN) $(LDLIBS)

test: $(TEST_BIN)
	$(TEST_BIN)


gcov_report: clean
	mkdir -p report $(LIB_DIR)
	$(CC) $(CFLAGS) $(GCOVFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/graph.c -o $(SRC_DIR)/graph.o
	ar rcs $(LIB) $(SRC_DIR)/graph.o
	$(CC) $(CFLAGS) $(GCOVFLAGS) -I$(INC_DIR) $(TEST_DIR)/test.c $(LIB) -o $(TEST_BIN) $(LDLIBS)
	./$(TEST_BIN) || true
	gcov -o $(SRC_DIR) $(SRC_DIR)/graph.c > report/coverage.txt
	@echo "Coverage written to report/coverage.txt"


leaks: $(TEST_BIN)
	valgrind --leak-check=full --error-exitcode=1 $(TEST_BIN)


clean:
	rm -rf $(OBJ) $(TEST_BIN) $(LIB_DIR) \
		$(SRC_DIR)/*.gcno $(SRC_DIR)/*.gcda $(SRC_DIR)/*.gcov \
		$(TEST_DIR)/*.gcno $(TEST_DIR)/*.gcda $(TEST_DIR)/*.gcov \
		*.gcno *.gcda *.gcov \
		report \
		test/run.*
