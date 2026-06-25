CC = gcc
CFLAGS = -Wall -Wextra -O3
INCLUDES = -Ichip_core/include
SRC_DIR = chip_core/src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests
TARGET = $(BIN_DIR)/emulator

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_ROM_URL = https://github.com/Klaus2m5/6502_65C02_functional_tests/raw/master/bin_files/6502_functional_test.bin
TEST_ROM = $(TEST_DIR)/6502_functional_test.bin

.PHONY: all clean setup download_test test

all: setup $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

setup:
	mkdir -p obj bin tests

download_test: setup
	@if [ ! -f $(TEST_ROM) ]; then \
		echo "Downloading Klaus Dormann's 6502 functional test binary..."; \
		curl -L -s $(TEST_ROM_URL) -o $(TEST_ROM); \
	fi

test: all download_test
	@echo "Launching emulator w/6502 functional test suite..."
	./$(TARGET) --test $(TEST_ROM)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean Complete"

deepclean: clean
	rm -rf $(TEST_DIR)
