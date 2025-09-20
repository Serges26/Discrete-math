CC=gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage

SRC = src/join.c
OBJ = $(SRC:.c=.o)


TEST_SRC = tests/test.c
TEST_BIN = tests/run


all: test gcov_report

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


test: $(LIB) $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) src/join.c -lcheck -lm -lpthread -o $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: CFLAGS += $(GCOVFLAGS)
gcov_report: clean
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c $(SRC) -o join.o
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c $(TEST_SRC) -o test.o
	$(CC) $(CFLAGS) $(GCOVFLAGS) join.o test.o -lcheck -lm -lpthread -o $(TEST_BIN)
	./$(TEST_BIN)
	lcov -t "join" -o coverage.info -c -d .
	lcov --remove coverage.info "*/tests/*" -o coverage.info
	genhtml -o report coverage.info


leaks: test
	leaks --atExit -- ./tests/run

clean:
	rm -f st *.gcda *.gcno *.o
	rm -f src/*.o src/*.a src/*.gcno src/*.gcda src/*.gcov coverage.info
	rm -f tests/run tests/*.o tests/*.gcno tests/*.gcda tests/*.gcov
	rm -rf report


.PHONY: all clean test gcov_report leaks
