# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Target binary name
TARGET = hms

# Source files (সব .c ফাইল src ফোল্ডার থেকে নেবে)
SRC = $(wildcard src/*.c)

# Default rule
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)