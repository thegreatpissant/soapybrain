include 	../Makefile.inc
DIRS		= common 
ALL_EXECS	= triangles ex15_1 glm_tests oculus
TEST_EXECS 	= 
OBJ_FILES 	= common
EDITOR_MESS 	=  *~ 
STD		= -std=c++11
LDLIBS          += 
DEBUG_FLAG	+= -g -O3 -Wall

all: $(OBJ_FILES) $(ALL_EXECS) 

common : force_look
	cd common; $(MAKE) all

triangles: triangles.cpp  common
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangles triangles.cpp $(STD) ./common/shader_utils.o $(LDLIBS)

ex15_1: ex15_1.cpp common 
	$(CXX) $(DEBUG_FLAG) -I. -o ex15_1 ex15_1.cpp $(STD) ./common/shader_utils.o $(LDLIBS) 

oculus: oculus.cpp common
	$(CXX) $(DEBUG_FLAG) -I. -o oculus oculus.cpp $(STD) ./common/shader_utils.o $(LDLIBS)  -I $(HOME)/include $(HOME)/include/libovr_nsb/libovr_nsb.so $(HOME)/include/gl-matrix.c/libgl-matrix.so

glm_tests: glm_tests.cpp
	$(CXX) -o glm_tests glm_tests.cpp
clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
