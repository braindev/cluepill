CC=clang
CFLAGS=-c
LDFLAGS=
SOURCES=main.c json.c args.c config.c util.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cluepill

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
