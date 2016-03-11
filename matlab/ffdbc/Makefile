TARGET=ffdbc
CFLAGS=-m64 -pipe -O3 -Wall -W -D_REENTRANT -fPIC -D_QT_NO_DEBUG -D_QT_CORE_LIB
INC=-I/usr/include/qt5 -I/usr/include/qt5/QtCore -I.
LIBS=-lQt5Core -lpthread -L/usr/lib/x86_64-linux-gnu
CC=g++

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS) 
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
