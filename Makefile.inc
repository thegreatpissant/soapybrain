#CC		= g++
#DEBUG_FLAG	= -g -Wall
#CXX		= g++
CXX             = clang++
DEBUG_FLAG	= -O2 -Wall
LDLIBS 		= -lGL -lglut -lGLEW -lm -lGLU 
EDITOR_MESS 	= *~
STD             = -std=c++11
SHELL		= /bin/bash
.SILENT		:
