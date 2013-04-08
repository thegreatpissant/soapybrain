include 	../Makefile.inc
DIRS		= common 
ALL_EXECS	= triangles
TEST_EXECS 	= 
OBJ_FILES 	= common
EDITOR_MESS 	=  *~ 
STD		= -std=c++11



common : force_look
	cd common; $(MAKE) all

triangles: triangles.cpp 
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangles triangles.cpp $(STD) ./common/shader_utils.o

clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

all: $(OBJ_FILES) $(ALL_EXECS) 

.PHONY: all clean

force_look :
	true
