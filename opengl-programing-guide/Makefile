include 	../Makefile.inc
DIRS		= common 
ALL_EXECS	= triangles ex15_1 exj_1 glm_tests oculus exj_2 exj_3
TEST_EXECS 	= 
OBJ_FILES 	= common
EDITOR_MESS 	=  *~ 
STD		= -std=c++0x
OCULUS_LIBS	= -ludev -lpthread -lXext -lX11 -lXinerama
DEBUG_FLAG	+= -g -O3 -Wall
OCULUS_SDK	= /home/thegreatpissant

all: $(OBJ_FILES) $(ALL_EXECS) 

common : force_look
	cd common; $(MAKE) all

triangles: triangles.cpp  common
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangles triangles.cpp $(STD) ./common/shader_utils.o $(LDLIBS)

ex15_1: ex15_1.cpp common 
	$(CXX) $(DEBUG_FLAG) -I. -o ex15_1 ex15_1.cpp $(STD) ./common/shader_utils.o $(LDLIBS) 

# These are my personal endeavors
exj_1: exj_1.cpp common 
	$(CXX) $(DEBUG_FLAG) -I. -o exj_1 exj_1.cpp $(STD) ./common/shader_utils.o $(LDLIBS) 

exj_2: exj_2.cpp common 
	$(CXX) $(DEBUG_FLAG) -I. -o exj_2 exj_2.cpp $(STD) ./common/shader_utils.o $(LDLIBS) 

exj_3: exj_3.cpp common 
	$(CXX) $(DEBUG_FLAG) -I. -o exj_3 exj_3.cpp $(STD) ./common/shader_utils.o $(LDLIBS) 


oculus: oculus.cpp common
	$(CXX) $(DEBUG_FLAG) -I$(OCULUS_SDK)/LibOVR/Include -o oculus oculus.cpp $(OCULUS_SDK)/LibOVR/Lib/Linux/Release/x86_64/libovr.a  ./common/shader_utils.o  $(LDLIBS) $(OCULUS_LIBS)

glm_tests: glm_tests.cpp
	$(CXX) -o glm_tests glm_tests.cpp
clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
