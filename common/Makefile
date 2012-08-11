makefile:

CC=g++
DEBUG_FLAG = -g -Wall
DIRS = common
ALL_EXECS = 
TEST_EXECS = 
OBJ_FILES = shader_utils.o
EDITOR_MESS =  *~
LDLIBS =-lGL -lglut -lGLEW

all: $(ALL_EXECS) $(OBJ_FILES)

shader_utils.o :
	gcc $(DEBUG_FLAG) $(LDLIBS) -c shader_utils.c

clean :
	rm *.o $(EDITOR_MESS) 2>/dev/null

.PHONY: all clean
