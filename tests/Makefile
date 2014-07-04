include ../Makefile.inc
DIRS		= 
ALL_EXECS	= test1
OBJ_FILES	=
EDITOR_MESS	=

CXX=clang++

all: $(DIRS) $(OBJ_FILES) $(ALL_EXECS)

common : force_look
	cd common; $(MAKE) all

test1: test1.cpp common
	$(CXX)  $(DEBUG_FLAG) -I. $(LDLIBS) -o test1 test1.cpp common/shader_utils.o

clean:
	rm -f *.o $(EDITOR_MESS) $(ALL_EXECS)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean

force_look :
	true
