include 	Makefile.inc
DIRS		= common shaders
ALL_EXECS_1	= triangle triangle2 triangle3 triangle4 cube5 cube6 cube6b
ALL_EXECS_2	= chptone chpttwo chpttwo.4 chptthree.1 chptthree.2 chptfour.1
ALL_EXECS 	= $(ALL_EXECS_1) $(ALL_EXECS_2)
TEST_EXECS 	= 
OBJ_FILES 	= common
EDITOR_MESS 	=  *~ 


all: $(ALL_EXECS) $(OBJ_FILES)

wikiglbook : $(ALL_EXECS_1)

openglbook : $(ALL_EXECS_2)

common : force_look
	cd common; $(MAKE) all

utils: utils.c Utils.h
	gcc $(DEBUG_FLAG) -I. $(LDLIBS) -c utils.c

chptone: chptone.c
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -o chptone chptone.c

chpttwo: chpttwo.c
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -o chpttwo chpttwo.c

chpttwo.4: chpttwo.4.c
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -o chpttwo.4 chpttwo.4.c

chptthree.1: chptthree.1.c
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -o chptthree.1 chptthree.1.c

chptthree.2: chptthree.2.c
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -o chptthree.2 chptthree.2.c

chptfour.1: chptfour.1.c utils
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -o chptfour.1 chptfour.1.c utils.o

triangle: triangle.c common
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangle triangle.c common/shader_utils.o

triangle2: triangle2.c common
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangle2 triangle2.c common/shader_utils.o

triangle3: triangle3.c common
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangle3 triangle3.c common/shader_utils.o

triangle4: triangle4.cpp common
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangle4 triangle4.cpp common/shader_utils.o

cube5: cube5.cpp common
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o cube5 cube5.cpp common/shader_utils.o

cube6: cube6.cpp res_texture.c common
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o cube6 cube6.cpp common/shader_utils.o

cube6b: cube6b.cpp res_texture.c common
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o cube6b cube6b.cpp common/shader_utils.o

obj7: obj7.cpp common
	$(CXX) $(DEBUG_FLAG) -I. $(LDLIBS) -o obj7 obj7.cpp common/shader_utils.o

clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
