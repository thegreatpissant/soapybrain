include 	../Makefile.inc
DIRS		= common shaders 
ALL_EXECS	= triangle triangle2 triangle3 triangle4 cube5 cube6 cube6b
TEST_EXECS 	= 
OBJ_FILES 	= common
EDITOR_MESS 	=  *~ 


all: $(ALL_EXECS) $(OBJ_FILES)

common : force_look
	cd common; $(MAKE) all

triangle: triangle.c common
	$(CXX) $(DEBUG_FLAG) -I. -o triangle triangle.c common/shader_utils.o $(LDLIBS) 

triangle2: triangle2.c common
	$(CXX) $(DEBUG_FLAG) -I. -o triangle2 triangle2.c common/shader_utils.o $(LDLIBS) 

triangle3: triangle3.c common
	$(CXX) $(DEBUG_FLAG) -I. -o triangle3 triangle3.c common/shader_utils.o $(LDLIBS) 

triangle4: triangle4.cpp common
	$(CXX) $(DEBUG_FLAG) -I. -o triangle4 triangle4.cpp common/shader_utils.o $(LDLIBS) 

cube5: cube5.cpp common
	mpic++ $(DEBUG_FLAG) -I. -o cube5 cube5.cpp common/shader_utils.o $(LDLIBS) 
	#$(CXX) $(DEBUG_FLAG) -I. -o cube5 cube5.cpp common/shader_utils.o $(LDLIBS) 

cube6: cube6.cpp res_texture.c common
	$(CXX) $(DEBUG_FLAG) -I. -o cube6 cube6.cpp common/shader_utils.o $(LDLIBS) 

cube6b: cube6b.cpp res_texture.c common
	$(CXX) $(DEBUG_FLAG) -I. -o cube6b cube6b.cpp common/shader_utils.o $(LDLIBS) 

obj7: obj7.cpp common
	$(CXX) $(DEBUG_FLAG) -I. -o obj7 obj7.cpp common/shader_utils.o $(LDLIBS) 

clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
