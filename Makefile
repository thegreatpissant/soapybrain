makefile:

CC=g++
DEBUG_FLAG = -g -Wall
DIRS = common
ALL_EXECS = triangle
TEST_EXECS = 
OBJ_FILES = common
EDITOR_MESS =  
LDLIBS =-lGL -lglut -lGLEW

all: $(ALL_EXECS) $(OBJ_FILES)

common : force_look
	cd common; $(MAKE) all

triangle: triangle.c common
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangle triangle.c common/shader_utils.o

clean:
	rm  *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true