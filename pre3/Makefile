makefile:

DEBUG_FLAG = -g -Wall
ALL_EXECS = chapterone chapterthree planets light
TEST_EXECS = 
OBJ_FILES = 
EDITOR_MESS =  
LIBS = -lGL -lglut -lGLU

all: $(ALL_EXECS)

chapterone: chapterone.c
	gcc $(DEBUG_FLAG) -I. $(LIBS) -o chapterone chapterone.c

chapterthree: chapterthree.c
	gcc $(DEBUG_FLAG) -I. $(LIBS) -o chapterthree chapterthree.c

planets: planets.c
	gcc $(DEBUG_FLAG) -I. $(LIBS) -o planets planets.c

light: light.c
	gcc $(DEBUG_FLAG) -I. $(LIBS) -o light light.c

modelrender: modelrender.c
	gcc $(DEBUG_FLAG) -I. $(LIBS) -o modelrender modelrender.c ../md5reader/md5reader.o ../md5reader/skeleton.o

clean:
	rm  $(ALL_EXECS) $(EDITOR_MESS)
