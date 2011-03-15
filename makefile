# DEMOS dans /usr/share/mesademos/progs/

CFLAGS = -O3 -I/usr/include/X11 -DSHM
XLIBS = -L/usr/X11/lib -lX11
CC = gcc


LIBDIR = /usr/lib

GL_LIB = $(LIBDIR)/libGL.a

GLU_LIB = $(LIBDIR)/libGLU.a

TK_LIB = $(LIBDIR)/libtk.a

AUX_LIB = $(LIBDIR)/libaux.a

INCDIR = /usr/include/GL

GL_LIBS = -L$(LIBDIR) -lglut -lGLU -lGL $(XLIBS) -lm

LIB_DEP = $(GL_LIB) $(GLU_LIB)

####Type in your list of executable names###
###(same filename as source file less .c)###

PROGS = rubix

##### RULES #####

.SUFFIXES:
.SUFFIXES: .c

.c: $(LIB_DEP)
	$(CC) -g -I$(INCDIR) $(CFLAGS) $< $(GL_LIBS) -o $@

##### TARGETS #####

default:
	@echo "Specify a target configuration"

clean:
	-rm *.o *~

realclean:
	-rm $(PROGS)
	-rm *.o *~

targets: $(PROGS)
