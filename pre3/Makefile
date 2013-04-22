makefile:

DEBUG_FLAG = -g -Wall
ALL_EXECS = chapterone chapterthree chapternine planets light
TEST_EXECS = 
OBJ_FILES = 
EDITOR_MESS =  
LIBS = -lGL -lglut -lGLU

all: $(ALL_EXECS)

chapterone: chapterone.c
	$(CC) $(DEBUG_FLAG) -I. -o chapterone chapterone.c $(LIBS)

chapterthree: chapterthree.c
	$(CC) $(DEBUG_FLAG) -I. -o chapterthree chapterthree.c $(LIBS) 

chapternine: chapternine.c
	$(CC) $(DEBUG_FLAG) -I. -o chapternine chapternine.c $(LIBS) 

planets: planets.c
	$(CC) $(DEBUG_FLAG) -I. -o planets planets.c $(LIBS) 

light: light.c
	$(CC) $(DEBUG_FLAG) -I.  -o light light.c $(LIBS) 

modelrender: modelrender.c
	$(CC) $(DEBUG_FLAG) -I. -o modelrender modelrender.c ../md5reader/md5reader.o ../md5reader/skeleton.o $(LIBS) 

clean:
	rm  $(ALL_EXECS) $(EDITOR_MESS)
