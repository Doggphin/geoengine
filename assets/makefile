
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=$(RES)
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Which compiler to use	
CC=g++
# Which flags to use
CFLAGS =-Wall
# Which libraries to use
LDFLAGS = 
# Which object files are part of the final program
OBJFILES= vector3.o Quaternion.o gltfloader.o comp_neonsign.o comp_sawblade.o comp_raycasttester.o openglhelpers.o drawer.o physics.o geometry.o comp_collider.o vector2.o comp_rigidbody.o window.o gizmodrawer.o comp_cameracontroller.o comp_mesh.o comp_camera.o comp_entityrotator.o comp_playercontroller.o input.o ecs.o project.o
# What to name the final prog
EXE = final

all: $(EXE)

fatal.o: fatal.cpp CSCIx229.h
errcheck.o: errcheck.cpp CSCIx229.h
print.o: print.cpp CSCIx229.h
loadtexbmp.o: loadtexbmp.cpp CSCIx229.h
loadobj.o: loadobj.cpp CSCIx229.h
projection.o: projection.cpp CSCIx229.h
project.o: project.cpp CSCIx229.h


CSCIx229.a: projection.o loadobj.o loadtexbmp.o print.o errcheck.o fatal.o
	ar -rcs $@ $^


.cpp.o:
	g++ -c $(CFLG)  $<

$(EXE): $(OBJFILES) CSCIx229.a
	$(CC) $(CLFG) $(CFLAGS) -o $(EXE) $(OBJFILES) CSCIx229.a jsoncpp.cpp $(LIBS) $(LDFLAGS)
#	$(CC) $(CLFG) $(CFLAGS) -o $(EXE) -g $(OBJFILES) CSCIx229.a $(LIBS) $(LDFLAGS)

clean:
	$(CLEAN)