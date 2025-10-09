
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O0 -Wno-gnu-zero-variadic-macro-arguments -Wno-newline-eof
GCOVFLAGS = -fprofile-arcs -ftest-coverage

SRC = src/clause.c
OBJ = build/clause.o
LIB = lib.a

TEST_SRC = tests/test.c
TEST_BIN = tests/run


CHECK_LIBS = -lcheck -pthread -lm


.PHONY: all test lib clean gcov_report main

all: test main


main: main.o $(OBJ)
	$(CC) $(CFLAGS) $(GCOVFLAGS) main.o $(OBJ) -o main
	./main

main.o: main.c
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c main.c -o main.o

lib: $(LIB)

$(LIB): build
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c $(SRC) -o $(OBJ)
	ar rcs $(LIB) $(OBJ)

build:
	mkdir -p build

$(TEST_BIN): lib $(TEST_SRC)
	$(CC) $(CFLAGS) $(GCOVFLAGS) $(TEST_SRC) -o $(TEST_BIN) $(LIB) $(CHECK_LIBS)

test: $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: test
	@echo "Generating gcov report..."
	lcov --directory . --capture --output-file coverage.info || true
	lcov --remove coverage.info '/usr/*' --output-file coverage.info.filtered || true
	genhtml coverage.info.filtered --output-directory coverage_report || true
	@echo "Report in coverage_report/index.html (if lcov/genhtml available)"

clean:
	rm -rf build $(TEST_BIN) *.gcda *.gcno coverage.info coverage.info.filtered coverage_report tests/*.gc* *.a main main.o
