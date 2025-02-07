SOURCES = main.c shellFunctions.c internalCommands.c initialise.c
CC = gcc
CFLAGS = -Wall -pedantic
TARGET = shell

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
