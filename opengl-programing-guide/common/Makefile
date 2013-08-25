include 	../../Makefile.inc

DIRS 		= 
ALL_EXECS 	= 
TEST_EXECS 	= 
OBJ_FILES 	= shader_utils.o

all: $(ALL_EXECS) $(OBJ_FILES)

shader_utils.o : shader_utils.cpp shader_utils.h
	$(CXX) $(DEBUG_FLAG) -c shader_utils.cpp

clean :
	rm -f *.o $(EDITOR_MESS)

.PHONY: all clean
