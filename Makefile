CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11 -g --coverage -Iinclude
LDFLAGS = --coverage
LIBDIR  = lib
LIBNAME = libzhegalkin.a
LIB     = $(LIBDIR)/$(LIBNAME)

SRC     = src/zhegalkin.c
OBJ     = $(SRC:.c=.o)

TEST_SRC = test/test.c
TEST_BIN = test/run

.PHONY: all clean test gcov_report

all: $(LIB)

$(LIB): $(OBJ)
	@mkdir -p $(LIBDIR)
	ar rcs $(LIB) $(OBJ)

src/%.o: src/%.c include/zhegalkin.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB) $(TEST_SRC)
	@mkdir -p test
	$(CC) $(CFLAGS) $(TEST_SRC) -L$(LIBDIR) -lzhegalkin -lcheck -lpthread -lm -o $(TEST_BIN) $(LDFLAGS)
	./$(TEST_BIN)

gcov_report: test
	@echo "Generating gcov report..."
	@gcov src/zhegalkin.c > /dev/null

clean:
	rm -f src/*.o
	rm -f *.gcno *.gcda *.gcov
	rm -f $(LIB)
	rm -f $(TEST_BIN)
	rm -rf test/run.*
	rm -rf test/run-test.*
	rm -rf src/zhegalkin.gc*