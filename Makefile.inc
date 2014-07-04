#CC		= g++
#DEBUG_FLAG	= -g -Wall
#CXX		= g++
CXX             = clang++
DEBUG_FLAG	= -O2 -Wall -stdlib=libc++ -lc++abi 
LDLIBS 		= -lGL -lglut -lGLEW -lm -lGLU 
EDITOR_MESS 	= *~

SHELL		= /bin/bash
.SILENT		:
