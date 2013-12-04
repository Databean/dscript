CC := g++ -std=gnu++11 -g
CFLAGS = -Wall -c
SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard $(INCL_HOME)/*.h)
OBJECTS := $(addprefix $(OBJ_HOME)/scriptlib/,$(SOURCES:.cpp=.o))
INCLUDES := -I$(INCL_HOME)

main:  $(OBJECTS)
	
$(OBJ_HOME)/scriptlib/%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $(OBJ_HOME)/scriptlib/$@ $(INCLUDES)

.PHONY: clean
clean:
	rm *.o