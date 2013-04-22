include 	../../Makefile.inc

DIRS 		= 
ALL_EXECS 	= 
TEST_EXECS 	= 
OBJ_FILES 	= shader_utils.o

all: $(ALL_EXECS) $(OBJ_FILES)

shader_utils.o :
	$(CXX) $(DEBUG_FLAG) -c shader_utils.cpp

clean :
	rm -f *.o $(EDITOR_MESS)

.PHONY: all clean
