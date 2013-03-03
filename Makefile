#------------------------------------------------------------
# Makefile for My GraphicLibrary, this require the support
# of c++11
# Author: Julian Wu
#------------------------------------------------------------

CC	= g++
C	= cpp
H	= h
CFLAGS		= -g -std=c++0x

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
    CC = clang++
#  LDFLAGS = -framework OpenGL
  CFLAGS = -g -std=c++11 -stdlib=libc++ 
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lGLU -lm
    CGLAGS	= -g -std=c++0x
  endif
endif


HFILES 	= Matrix.${H} Vector.${H} Utility.${H} Camera.${H} GraphicUtilities.${H} GLCommonHeader.${H} GraphicObject.${H} GraphicMath.${H} GraphicCamera.${H}
OFILES 	= Matrix.o Vector.o Utility.o Camera.o GraphicUtilities.o GraphicMath.o GraphicObject.o GraphicCamera.o
	
PROJECT = main_base

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

Camera.o: Camera.${C} Camera.${H} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Camera.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Matrix.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}

GraphicUtilities.o: GraphicUtilities.${C} GraphicUtilities.${H} 
	${CC} $(CFLAGS) -c GraphicUtilities.${C}

GraphicObject.o: GraphicObject.${C} GraphicObject.${H} 
	${CC} $(CFLAGS) -c GraphicObject.${C}

GraphicMath.o: GraphicMath.${C} GraphicMath.${H}
	${CC} $(CFLAGS) -c GraphicMath.${C}

GraphicCamera.o: GraphicCamera.${C} GraphicCamera.${H}
	${CC} $(CFLAGS) -c GraphicCamera.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}
