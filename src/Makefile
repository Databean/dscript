CC := g++ -std=gnu++11 -g
CFLAGS = -Wall -c
SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard $(INCL_HOME)/*.h)
OBJECTS := $(addprefix $(OBJ_HOME)/,$(SOURCES:.cpp=.o) lex.yy.o y.tab.o)
INCLUDES := -I$(INCL_HOME)

main:  y.tab.c lex.yy.c $(OBJECTS)
	cd scriptlib && $(MAKE)
	

$(INCL_HOME)/scanner.h lex.yy.c: scanner.l
	flex  --header-file=../include/scanner.h scanner.l

y.tab.c y.tab.h: parser.y
	bison -dvty parser.y

$(OBJ_HOME)/%.o: %.cpp $(HEADERS) $(INCL_HOME)/scanner.h y.tab.h
	$(CC) $(CFLAGS) $< -o $@ $(INCLUDES)

$(OBJ_HOME)/%.o: %.c $(HEADERS) y.tab.h
	$(CC) $(CFLAGS) $< -o $@ $(INCLUDES)

.PHONY: clean
clean:
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f y.output