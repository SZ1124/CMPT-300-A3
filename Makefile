# Cross-compiler and compiler flags
CC = gcc
CFLAGS = -std=c99 -Wall -g -D_POSIX_C_SOURCE=2000809L -pthread

# Source files, object files, and header files
SRC = commands.c main.c
OBJ = $(SRC:.c=.o) list.o
HDR = commands.h list.h

# Executable name
TARGET = AS3

# Rule to build the executable and install
all: $(TARGET) 

# Rule to build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files from source files
%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(filter-out list.o, $(OBJ)) $(TARGET)