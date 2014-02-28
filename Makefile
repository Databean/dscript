CC := g++ -std=gnu++11 -g -O2
export OBJ_HOME := $(realpath obj)
export INCL_HOME := $(realpath include)
EXECUTABLE := dscript

$(EXECUTABLE): 
	cd src && $(MAKE)
	$(CC) -Wall obj/*.o obj/scriptlib/*.o -o $(EXECUTABLE)

.PHONY: clean
clean: 
	rm -f obj/*.o
	rm -f obj/scriptlib/*.o
	rm -f $(EXECUTABLE)
	cd src && $(MAKE) clean
