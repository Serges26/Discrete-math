CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11 -g --coverage -Iinclude -Isrc
LDFLAGS = --coverage

SRC_DIR  = src
INC_DIR  = include
TEST_DIR = test
LIB_DIR  = lib

LIB_NAME = $(LIB_DIR)/libboolean_forms.a
SRC      = $(SRC_DIR)/boolean_forms.c
OBJ      = $(SRC:.c=.o)

TEST_SRC = $(TEST_DIR)/test.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_BIN = $(TEST_DIR)/run

.PHONY: all test gcov_report clean fclean re

all: $(LIB_NAME) $(TEST_BIN)

$(LIB_NAME): $(OBJ)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/boolean_forms.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c $(INC_DIR)/boolean_forms.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_OBJ) $(LIB_NAME)
	@mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJ) $(LIB_NAME) -lcheck -lpthread -lm $(LDFLAGS)

test: $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: fclean all test
	@mkdir -p report
	lcov -t "boolean_forms" -o report/coverage.info -c -d .
	genhtml -o report report/coverage.info

clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o
	rm -f $(SRC_DIR)/*.gcno $(SRC_DIR)/*.gcda
	rm -f $(TEST_DIR)/*.gcno $(TEST_DIR)/*.gcda
	rm -rf test/run
	rm -rf lib/*


re: clean all
