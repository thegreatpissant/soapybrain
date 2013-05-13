include 	../Makefile.inc
DIRS		= common 
ALL_EXECS	= triangles ex15_1
TEST_EXECS 	= 
OBJ_FILES 	= common
EDITOR_MESS 	=  *~ 
STD		= -std=c++11
LDLIBS          += libovr_nsb/libovr_nsb.a gl-matrix.c/libgl-matrix.a -lpthread -lm libovr_nsb/libovr_nsb.so

all: $(OBJ_FILES) $(ALL_EXECS) 

common : force_look
	cd common; $(MAKE) all

triangles: triangles.cpp 
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangles triangles.cpp $(STD) ./common/shader_utils.o

ex15_1: ex15_1.cpp 
	$(CXX) $(DEBUG_FLAG) -I. -Igl-matrix.c -Ilibovr_nsb -o ex15_1 ex15_1.cpp $(STD) ./common/shader_utils.o $(LDLIBS)

clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
