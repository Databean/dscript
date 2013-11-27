CC := g++ -std=gnu++11 -g
CFLAGS = -Wall -c
SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard $(INCL_HOME)/*.h)
OBJECTS := $(SOURCES:.cpp=.o)
INCLUDES := -I$(INCL_HOME)

main:  $(OBJECTS)
	
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $(OBJ_HOME)/scriptlib/$@ $(INCLUDES)

.PHONY: clean
clean:
	rm *.o