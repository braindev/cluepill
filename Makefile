CC=clang
CFLAGS=-c -g
LDFLAGS=
SOURCES=main.c json.c args.c config.c util.c monitor.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=cluepill

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
