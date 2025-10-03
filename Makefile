CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage


SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
REPORT_DIR = report


ITER_H = $(SRC_DIR)/iter.h
ITER_SRC = $(SRC_DIR)/iterator_permut.c
MAIN_SRC = main.c
TEST_SRC = $(TEST_DIR)/test.c


LIB = $(BUILD_DIR)/libiter.a
MAIN_BIN = $(BUILD_DIR)/main
TEST_BIN = $(TEST_DIR)/run


CHECK_LIBS = `pkg-config --cflags --libs check`


all: $(MAIN_BIN)


lib: $(LIB)

$(LIB): $(ITER_SRC) $(ITER_H) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(ITER_SRC) -o $(BUILD_DIR)/iterator_permut.o
	ar rcs $@ $(BUILD_DIR)/iterator_permut.o


$(MAIN_BIN): $(MAIN_SRC) $(LIB)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(MAIN_SRC) -L$(BUILD_DIR) -liter -o $@
	./build/main


test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) $(ITER_SRC) $(ITER_H)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(TEST_SRC) $(ITER_SRC) $(CHECK_LIBS) -o $@


gcov_report: clean | $(BUILD_DIR) $(REPORT_DIR)
	# Компиляция с покрытием
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -I$(SRC_DIR) $(ITER_SRC) $(TEST_SRC) $(CHECK_LIBS) -o $(TEST_BIN)
	# Запуск тестов
	./$(TEST_BIN)
	# Генерация отчета
	gcovr -r . --html --html-details -o $(REPORT_DIR)/report.html
	@gcovr -r . --print-summary
	@echo "Отчет покрытия: $(REPORT_DIR)/report.html"


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(REPORT_DIR):
	mkdir -p $(REPORT_DIR)


clean:
	rm -rf $(BUILD_DIR) $(REPORT_DIR) $(TEST_BIN)
	rm -rf tests/run.*
	rm -f *.gcda *.gcno *.gcov
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
	find . -name "*.gcov" -delete


.PHONY: all lib test gcov_report clean