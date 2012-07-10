makefile:

all: chapterone chapterthree planets

chapterone: chapterone.cc
	gcc -g -I. -lGL -lglut -lGLU -o chapterone chapterone.cc

chapterthree: chapterthree.cc
	gcc -g -I. -lGL -lglut -lGLU -o chapterthree chapterthree.cc

planets: planets.cc
	gcc -g -I. -lGL -lglut -lGLU -o planets planets.cc

clean:
	rm -f planets chapterone chapterthree