# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lGLU -lGL -lX11

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Target executable
TARGET = main

# Find all .c files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate corresponding .o files in the obj directory
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile .c files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Phony targets
.PHONY: all clean