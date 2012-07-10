makefile:

DEBUG_FLAG = -g
ALL_EXECS = chapterone chapterthree planets
TEST_EXECS = 
OBJ_FILES = 
EDITOR_MESS = *~ \#*

all: $(ALL_EXECS)

chapterone: chapterone.cc
	gcc $(DEBUG_FLAG) -I. -lGL -lglut -lGLU -o chapterone chapterone.cc

chapterthree: chapterthree.cc
	gcc $(DEBUG_FLAG) -I. -lGL -lglut -lGLU -o chapterthree chapterthree.cc

planets: planets.cc
	gcc $(DEBUG_FLAG) -I. -lGL -lglut -lGLU -o planets planets.cc

clean:
	rm -f $(ALL_EXECS) $(EDITOR_MESS)