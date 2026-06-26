SHELL = /usr/bin/sh
CC = gcc
CFLAGS = -Wall -Wextra -O3
INCLUDES = -Ichip_core/include
SRC_DIR = chip_core/src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests
TARGET = $(BIN_DIR)/emulator

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/cpu.c $(SRC_DIR)/memory.c
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
	-mkdir obj
	-mkdir bin
	-mkdir tests

download_test: setup
	@echo "Downloading Klaus Dormann's 6502 functional test binary..."
	curl -L -s -o $(TEST_ROM) $(TEST_ROM_URL)

test: all download_test
	@echo "Launching emulator w/6502 functional test suite..."
	./$(TARGET) --test $(TEST_ROM)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean Complete"

deepclean: clean
	rm -rf $(TEST_DIR)
