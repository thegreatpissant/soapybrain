include 	../Makefile.inc
DIRS		= 
ALL_EXECS_2	= chptone chpttwo chpttwo.4 chptthree.1 chptthree.2 chptfour.1
ALL_EXECS 	= $(ALL_EXECS_2)
TEST_EXECS 	= 
OBJ_FILES 	= 
EDITOR_MESS 	=  *~ 


all: $(ALL_EXECS) $(OBJ_FILES) $(DIRS)

utils: utils.c Utils.h
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -c utils.c

chptone: chptone.c
	$(CC) $(DEBUG_FLAG) -I. -o chptone chptone.c  $(LDLIBS) 

chpttwo: chpttwo.c
	$(CC) $(DEBUG_FLAG) -I. -o chpttwo chpttwo.c $(LDLIBS) 

chpttwo.4: chpttwo.4.c
	$(CC) $(DEBUG_FLAG) -I. -o chpttwo.4 chpttwo.4.c $(LDLIBS) 

chptthree.1: chptthree.1.c
	$(CC) $(DEBUG_FLAG) -I. -o chptthree.1 chptthree.1.c $(LDLIBS) 

chptthree.2: chptthree.2.c
	$(CC) $(DEBUG_FLAG) -I. -o chptthree.2 chptthree.2.c  $(LDLIBS) 

chptfour.1: chptfour.1.c utils
	$(CC) $(DEBUG_FLAG) -I. -o chptfour.1 chptfour.1.c utils.o $(LDLIBS) 

clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
