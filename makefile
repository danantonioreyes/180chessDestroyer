CC = gcc
CFLAGS = -lm
SOURCES = chesstpl.c
HEADERS = shiva.h
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = test

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)

.c.o: 
	$(CC) $< -c $(CFLAGS) 

clean: 
	rm $(OBJECTS)
	rm $(EXECUTABLE)

rebuild: clean all

run:
	./$(EXECUTABLE)
