CC=clang
CFLAGS=-c
LDFLAGS=
SOURCES=cluepill.c json.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cluepill

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
