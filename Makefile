CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -Iinclude -Isrc -g --coverage
LDFLAGS=--coverage

LIBDIR=lib
SRCDIR=src
INCDIR=include
TESTDIR=test

LIB=$(LIBDIR)/graph.a
TEST_EXE=$(TESTDIR)/run

SRC=$(SRCDIR)/graph.c
OBJ=$(SRC:.c=.o)

all: $(LIB) $(TEST_EXE)


$(LIBDIR):
	mkdir -p $(LIBDIR)


$(LIB): $(OBJ) | $(LIBDIR)
	ar rcs $(LIB) $(OBJ)

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/graph.h $(SRCDIR)/internal.h
	$(CC) $(CFLAGS) -c $< -o $@


$(TEST_EXE): $(TESTDIR)/test.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $(TESTDIR)/test.c $(LIB) -lcheck -lpthread -lm $(LDFLAGS)

test: $(TEST_EXE)
	./$(TEST_EXE)

gcov_report: test
	@mkdir -p report
	gcov -b -c $(SRCDIR)/*.c > /dev/null || true
	lcov --capture --directory . --output-file coverage.info >/dev/null 2>&1 || true
	genhtml coverage.info --output-directory report >/dev/null 2>&1 || true
	@echo "Coverage report (if lcov is installed) in ./report"

clean:
	rm -f $(SRCDIR)/*.o $(SRCDIR)/*.gcda $(SRCDIR)/*.gcno *.gcda *.gcno *.gcov coverage.info
	rm -f $(TESTDIR)/*.gcda $(TESTDIR)/*.gcno
	rm -f $(TEST_EXE)
	rm -f $(LIB)
	rm -rf report
	rm -rf test/run.*
.PHONY: all test clean gcov_report
