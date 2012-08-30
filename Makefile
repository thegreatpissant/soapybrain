include 	Makefile.inc
DIRS		= pre3 opengl-wikibook
ALL_EXECS_2	= chptone chpttwo chpttwo.4 chptthree.1 chptthree.2 chptfour.1
ALL_EXECS 	= $(ALL_EXECS_2)
TEST_EXECS 	= 
OBJ_FILES 	= 
EDITOR_MESS 	=  *~ 


all: $(ALL_EXECS) $(OBJ_FILES) $(DIRS)

opengl-wikibook : force_look
	cd opengl-wikibook; $(MAKE) all

pre3 : force_look
	cd pre3; $(MAKE) all

openglbook : $(ALL_EXECS_2)

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

clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
