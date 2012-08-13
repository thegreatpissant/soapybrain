include 	../Makefile.inc

DIRS 		= 
ALL_EXECS 	= 
TEST_EXECS 	= 
OBJ_FILES 	= shader_utils.o

all: $(ALL_EXECS) $(OBJ_FILES)

shader_utils.o :
	$(CC) $(DEBUG_FLAG) $(LDLIBS) -c shader_utils.c

clean :
	rm -f *.o $(EDITOR_MESS)

.PHONY: all clean
