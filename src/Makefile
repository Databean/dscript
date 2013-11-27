CC := g++ -std=gnu++11 -g
CFLAGS = -Wall -c
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o) lex.yy.o y.tab.o

main:  y.tab.c lex.yy.c $(OBJECTS)
	cd scriptlib && $(MAKE)
	

lex.yy.c: scanner.l
	flex scanner.l

y.tab.c: parser.y
	bison -dvty parser.y

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $(OBJ_HOME)/$@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $(OBJ_HOME)/$@ -Wall

.PHONY: clean
clean:
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f y.output