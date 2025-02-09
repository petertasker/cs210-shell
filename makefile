SOURCES = shell.c shell_library.c built_in_commands.c initialise.c linked_list.c
CC = gcc
CFLAGS = -Wall -pedantic
TARGET = shell

# Corrected 'all' target
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
