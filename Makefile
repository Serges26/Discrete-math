CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11 -g --coverage -Iinclude
LDFLAGS = --coverage
LIBDIR  = lib
LIBNAME = libcoloring.a
LIB     = $(LIBDIR)/$(LIBNAME)

SRC     = src/coloring.c
OBJ     = $(SRC:.c=.o)

TEST_SRC = test/test.c
TEST_BIN = test/run

.PHONY: all clean test gcov_report

all: $(LIB)

$(LIB): $(OBJ)
	@mkdir -p $(LIBDIR)
	ar rcs $(LIB) $(OBJ)

src/%.o: src/%.c include/coloring.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB) $(TEST_SRC)
	@mkdir -p test
	$(CC) $(CFLAGS) $(TEST_SRC) -L$(LIBDIR) -lcoloring -lcheck -lpthread -lm -o $(TEST_BIN) $(LDFLAGS)
	./$(TEST_BIN)

gcov_report: test
	@gcov src/coloring.c > /dev/null

clean:
	rm -f src/*.o
	rm -f *.gcno *.gcda *.gcov
	rm -f $(LIB)
	rm -f $(TEST_BIN)
	rm -rf test/run.* test/run-test.*
	rm -rf src/*.gcno src/*.gcda
