#------------------------------------------------------------
# Makefile for My GraphicLibrary, this require the support
# of c++11
# Author: Julian Wu
#------------------------------------------------------------

CC	= g++
C	= cpp
H	= h
CFLAGS		= -g -std=c++0x -w

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
    CC = clang++
#  LDFLAGS = -framework OpenGL
  CFLAGS = -g -std=c++11 -stdlib=libc++ 
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -L/usr/lib64 -O2 -lX11 -lglut -lGL -lGLU -lm -lXmu
    CGLAGS	= -g -std=c++0x -w
  endif
endif


HFILES 	= GraphicUtilities.${H} GLCommonHeader.${H} GraphicObject.${H} GraphicMath.${H} GraphicCamera.${H} GraphicModel.${H}
OFILES 	= GraphicUtilities.o GraphicMath.o GraphicObject.o GraphicCamera.o GraphicModel.o
	
PROJECT = main_base

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

GraphicUtilities.o: GraphicUtilities.${C} GraphicUtilities.${H} 
	${CC} $(CFLAGS) -c GraphicUtilities.${C}

GraphicObject.o: GraphicObject.${C} GraphicObject.${H} 
	${CC} $(CFLAGS) -c GraphicObject.${C}

GraphicMath.o: GraphicMath.${C} GraphicMath.${H}
	${CC} $(CFLAGS) -c GraphicMath.${C}

GraphicCamera.o: GraphicCamera.${C} GraphicCamera.${H}
	${CC} $(CFLAGS) -c GraphicCamera.${C}

GraphicModel.o: GraphicModel.${C} GraphicModel.${H}
	${CC} $(CFLAGS) -c GraphicModel.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}
