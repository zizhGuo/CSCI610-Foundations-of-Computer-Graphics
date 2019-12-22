#
# Created by gmakemake (Ubuntu Sep 17 2019) on Fri Dec  6 12:05:17 2019
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

#
# This header.mak file sets up all the necessary options for compiling
# and linking C and C++ programs which use OpenGL, GLFW, and/or GLEW on
# the CS Ubuntu systems.  To use it, place it in the same directory as
# your source code, and run the command
#
#	gmakemake > Makefile
#
# To compile and link your program, just run "make".
#

# locations of important directories if the header files
# and library files aren't in the standard places
INCLUDE =
LIBDIRS =

# uncomment this if you get "-lGL not found"
# errors when linking on the CS machines
# LIBDIRS = -L/home/course/cscix10/lib/links

# common linker options
LDLIBS = -lSOIL -lGL -lGLEW -lglfw -lm

# language-specific linker options
CLDLIBS =
CCLDLIBS =

# compiler flags
CCFLAGS = -g $(INCLUDE) -DGL_GLEXT_PROTOTYPES
CFLAGS = -std=c99 $(CCFLAGS)
CXXFLAGS = $(CCFLAGS)

# linker flags
LIBFLAGS = -g $(LIBDIRS) $(LDLIBS)
CLIBFLAGS = $(LIBFLAGS) $(CLDLIBS)
CCLIBFLAGS = $(LIBFLAGS) $(CCLDLIBS)

########## End of flags from header.mak


CPP_FILES =	Application.cpp Buffers.cpp Canvas.cpp Cylinder.cpp Lighting.cpp Quad.cpp ShaderSetup.cpp Sphere.cpp Textures.cpp Utils.cpp Vector.cpp Viewing.cpp main.cpp
C_FILES =	
PS_FILES =	
S_FILES =	
H_FILES =	Application.h Buffers.h Canvas.h Color.h Cylinder.h CylinderData.h Lighting.h Normal.h Quad.h QuadData.h SOIL.h ShaderSetup.h Shapes.h Sphere.h SphereData.h TexCoord.h Textures.h Utils.h Vector.h Vertex.h Viewing.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	Application.o Buffers.o Canvas.o Cylinder.o Lighting.o Quad.o ShaderSetup.o Sphere.o Textures.o Utils.o Vector.o Viewing.o 

#
# Main targets
#

all:	main 

main:	main.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o main main.o $(OBJFILES) $(CCLIBFLAGS)

#
# Dependencies
#

Application.o:	Application.h Buffers.h Canvas.h Color.h Cylinder.h Lighting.h Normal.h Quad.h ShaderSetup.h Shapes.h Sphere.h TexCoord.h Textures.h Utils.h Vertex.h Viewing.h
Buffers.o:	Buffers.h Canvas.h Color.h Normal.h TexCoord.h Utils.h Vertex.h
Canvas.o:	Canvas.h Color.h Normal.h TexCoord.h Vector.h Vertex.h
Cylinder.o:	Canvas.h Color.h Cylinder.h CylinderData.h Normal.h TexCoord.h Vertex.h
Lighting.o:	Lighting.h Shapes.h Utils.h
Quad.o:	Canvas.h Color.h Normal.h Quad.h QuadData.h TexCoord.h Vertex.h
ShaderSetup.o:	ShaderSetup.h
Sphere.o:	Canvas.h Color.h Normal.h Sphere.h SphereData.h TexCoord.h Vertex.h
Textures.o:	Shapes.h Textures.h Utils.h
Utils.o:	Application.h Buffers.h Canvas.h Color.h Normal.h TexCoord.h Utils.h Vertex.h
Vector.o:	Vector.h
Viewing.o:	Utils.h Vector.h Viewing.h
main.o:	Application.h Buffers.h Canvas.h Color.h Normal.h ShaderSetup.h TexCoord.h Vertex.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) main.o core

realclean:        clean
	-/bin/rm -f main 
