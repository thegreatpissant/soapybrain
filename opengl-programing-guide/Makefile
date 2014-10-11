include 	../Makefile.inc
DIRS		= common 
ALL_EXECS	= triangles ex15_1 exj_1 glm_tests oculus_scafolding exj_2 exj_3 exj_4 exj_5
TEST_EXECS 	= 
OBJ_FILES 	= common
EDITOR_MESS 	=  *~
OCULUS_LIBS	= -ludev -lpthread -lXext -lX11 -lXinerama -lXrandr -lXxf86vm
DEBUG_FLAG	+= -g -O3 -Wall
OCULUS_SDK	= /home/thegreatpissant/src/OculusSDKv3.2
ENGINE_INCLUDES = ./common/Actor.h ./common/Render.h ./common/Camera.h ./common/Model.h ./common/Entity.h ./common/Display.h ./common/Shader.h
ENGINE_LIBS	= ./common/Shader.o
MODEL_LIBS	= ./common/Model_vbotorus.o
MODEL_INCLUDES  = ./common/Model_vbotorus.h

all: $(OBJ_FILES) $(ALL_EXECS) 

common : force_look
	cd common; $(MAKE) all

triangles: triangles.cpp  common
	$(CXX) $(DEBUG_FLAG) -I. -o triangles triangles.cpp $(STD) ./common/shader_utils.o $(LDLIBS)

ex15_1: ex15_1.cpp common 
	$(CXX) $(DEBUG_FLAG) -I. -o ex15_1 ex15_1.cpp $(STD) ./common/shader_utils.o $(LDLIBS)

# These are my personal endeavors
exj_1: exj_1.cpp common 
	$(CXX) $(DEBUG_FLAG) -I. -o exj_1 exj_1.cpp $(STD) ./common/shader_utils.o $(LDLIBS) 

exj_2: exj_2.cpp common 
	$(CXX) $(DEBUG_FLAG) -I. -o exj_2 exj_2.cpp $(STD) ./common/shader_utils.o $(LDLIBS) 

exj_3: exj_3.cpp common $(ENGINE_INCLUDES)
	$(CXX) $(DEBUG_FLAG) -I. -o exj_3 exj_3.cpp $(STD) ./common/shader_utils.o $(LDLIBS) 

exj_4: exj_4.cpp common $(ENGINE_INCLUDES)
	$(CXX) $(DEBUG_FLAG) -I. -o exj_4 exj_4.cpp $(STD) ./common/shader_utils.o $(LDLIBS) 

exj_5: exj_5.cpp common $(ENGINE_INCLUDES)
	$(CXX) $(STD) $(DEBUG_FLAG) -I. -I$(MODEL_INCLUDES) -o exj_5 exj_5.cpp $(STD) $(LDLIBS) $(ENGINE_LIBS) $(MODEL_LIBS)

oculus_scafolding: oculus_scafolding.cpp common
	$(CXX) $(STD) $(DEBUG_FLAG) -I$(OCULUS_SDK)/LibOVR/Src -o oculus_scafolding oculus_scafolding.cpp $(STD) $(OCULUS_SDK)/LibOVR/Lib/Linux/Debug/x86_64/libovr.a  ./common/shader_utils.o  $(LDLIBS) $(OCULUS_LIBS)

oculus: oculus.cpp common
	$(CXX) $(STD) $(DEBUG_FLAG) -I$(OCULUS_SDK)/LibOVR/Include -o oculus oculus.cpp $(STD) $(OCULUS_SDK)/LibOVR/Lib/Linux/Release/x86_64/libovr.a  ./common/shader_utils.o  $(LDLIBS) $(OCULUS_LIBS)

glm_tests: glm_tests.cpp
	$(CXX) -o glm_tests glm_tests.cpp
clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
