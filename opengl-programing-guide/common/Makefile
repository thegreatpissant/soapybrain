include 	../../Makefile.inc

DIRS 		= 
ALL_EXECS 	= 
TEST_EXECS 	= 
OBJ_FILES 	= shader_utils Shader Model_vbotorus

all: $(ALL_EXECS) $(OBJ_FILES)

shader_utils : shader_utils.cpp
	$(CXX) $(STD) $(DEBUG_FLAG) -c shader_utils.cpp

Shader : Shader.cpp Shader.h
	$(CXX) $(STD) $(DEBUG_FLAG) -c Shader.cpp

Model_vbotorus : Model_vbotorus.cpp Model_vbotorus.h
	$(CXX) $(STD) $(DEBUG_FLAG) -c Model_vbotorus.cpp

clean :
	rm -f *.o $(EDITOR_MESS)

.PHONY: all clean
