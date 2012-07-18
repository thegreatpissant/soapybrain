makefile:

DEBUG_FLAG = -g -Wall
ALL_EXECS = chapterone chapterthree planets modelrender
TEST_EXECS = 
OBJ_FILES = 
EDITOR_MESS = *~ \#*
LIBS = -lGL -lglut -lGLU
all: $(ALL_EXECS)

chapterone: chapterone.cc
	gcc $(DEBUG_FLAG) -I. $(LIBS) -o chapterone chapterone.cc

chapterthree: chapterthree.cc
	gcc $(DEBUG_FLAG) -I. $(LIBS) -o chapterthree chapterthree.cc

planets: planets.cc
	gcc $(DEBUG_FLAG) -I. $(LIBS) -o planets planets.cc

light: light.c
	gcc $(DEBUG_FLAG) -I. $(LIBS) -o light light.c

modelrender: modelrender.c
	gcc $(DEBUG_FLAG) -I. $(LIBS) -o modelrender modelrender.c ../md5reader/md5reader.o ../md5reader/skeleton.o
clean:
	rm -f $(ALL_EXECS) $(EDITOR_MESS)