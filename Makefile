#------------------------------------------------------------
# Makefile for My GraphicLibrary, this require the support
# of c++11
# Author: Julian Wu
#------------------------------------------------------------

CC = g++
C	= cpp
H	= h
CFLAGS 	= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lGLU -lm -lXmu -lX11 -O2
  endif
endif

HFILES 	= GraphicUtilities.${H} GLCommonHeader.${H} GraphicMath.${H}
OFILES 	= GraphicUtilities.o GraphicMath.o 
PROJECT = main_base

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

GraphicUtilities.o: GraphicUtilities.${C} GraphicUtilities.${H} GraphicMath.${H} 
	${CC} $(CFLAGS) -c GraphicUtilities.${C}

GraphicMath.o: GraphicMath.${C} GraphicMath.${H} 
	${CC} $(CFLAGS) -c GraphicMath.${C}
l  
debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}
