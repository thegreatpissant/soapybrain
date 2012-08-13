include 	Makefile.inc
DIRS		= common shaders
ALL_EXECS	= triangle2 triangle3 triangle4
TEST_EXECS 	= 
OBJ_FILES 	= common
EDITOR_MESS 	=  *~ 


all: $(ALL_EXECS) $(OBJ_FILES)

common : force_look
	cd common; $(MAKE) all

triangle2: triangle2.c common
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangle2 triangle2.c common/shader_utils.o

triangle3: triangle3.c common
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangle3 triangle3.c common/shader_utils.o

triangle4: triangle4.cpp common
	$(CC) $(DEBUG_FLAG) -I. $(LDLIBS) -o triangle4 triangle4.cpp common/shader_utils.o

clean:
	rm -f *.o $(ALL_EXECS) $(EDITOR_MESS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
