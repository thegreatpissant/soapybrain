include 	Makefile.inc
DIRS		= pre3 opengl-wikibook openglbook opengl-programing-guide
ALL_EXECS 	= 
TEST_EXECS 	= 
OBJ_FILES 	= 
EDITOR_MESS 	=  *~ 


all: $(ALL_EXECS) $(OBJ_FILES) $(DIRS)

opengl-wikibook : force_look
	cd opengl-wikibook; $(MAKE) all

openglbook : force_look
	cd openglbook; $(MAKE) all

pre3 : force_look
	cd pre3; $(MAKE) all

clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
