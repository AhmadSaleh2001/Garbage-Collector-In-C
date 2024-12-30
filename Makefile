# Define the compiler
CC = gcc -g

# Define the target executable
TARGET = main.exe

# Define source files
SRC = main.c MLD.c object_db.c

# Define compilation flags
CFLAGS = -Wall -Wextra -O2

# Default target
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Clean rule
clean:
	@echo "Cleaning up..."
ifeq ($(OS),Windows_NT)
	del $(TARGET)
else
	rm -f $(TARGET)
endif

.PHONY: all clean
