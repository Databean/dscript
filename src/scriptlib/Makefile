CC := g++ -std=gnu++11 -g
CFLAGS = -Wall -c
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

main:  $(OBJECTS)
	

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $(OBJ_HOME)/scriptlib/$@ -I..

.PHONY: clean
clean:
	rm *.o