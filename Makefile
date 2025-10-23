CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage

SRC = src/iterator.c
OBJ = $(SRC:.c=.o)

TEST_SRC = test/test.c
TEST_BIN = test/run

LIB_DIR = lib
LIB = $(LIB_DIR)/iterator.a


all: test

lib: $(LIB)


$(LIB): $(OBJ)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB) $(TEST_SRC)
	@mkdir -p $(dir $(TEST_BIN))
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB) -lcheck -lm -lpthread -o $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: CFLAGS += $(GCOVFLAGS)
gcov_report: clean
	@mkdir -p $(dir $(TEST_BIN))
	@mkdir -p $(LIB_DIR)
	$(CC) $(CFLAGS) -c $(SRC) -o iterator.o
	$(CC) $(CFLAGS) -c $(TEST_SRC) -o test.o
	$(CC) $(CFLAGS) iterator.o test.o -lcheck -lm -lpthread -o $(TEST_BIN)
	./$(TEST_BIN)
	lcov -t "iterator" -o coverage.info -c -d .
	lcov --remove coverage.info "*/test/*" -o coverage.info
	genhtml -o report coverage.info

leaks: test
	leaks --atExit -- ./test/run

clean:
	rm -f st *.gcda *.gcno *.o
	rm -f src/*.o src/*.a src/*.gcno src/*.gcda src/*.gcov coverage.info
	rm -f test/run test/*.o test/*.gcno test/*.gcda test/*.gcov
	rm -rf report
	rm -rf lib

.PHONY: all lib clean test gcov_report leaks
