SOURCES = src/shell.c src/shell_library.c src/built_in_commands.c src/initialise.c src/linked_list.c
CC = gcc
CFLAGS = -Wall -pedantic
TARGET = shell

# Corrected 'all' target
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
