EXE 		= particles

SRC 		= $(wildcard src/*.cpp)
OBJ 		= $(subst src, build, $(patsubst %.cpp, %.o, $(SRC)))

DBG 		= 	# debug flags

INCLUDE 	= -I include
LIB 		= -L lib/SFML_static -L lib/Other -lsfml-graphics-s -lsfml-window-s -lsfml-system-s lib/Other/glew32s.lib -lopengl32 -lfreetype -lwinmm -lgdi32 -lcomdlg32	# this is for static linking SFML
EXTRA		= -Werror -Wall -Wno-narrowing
STATIC  	= -static-libgcc -static-libstdc++ -static 	# for static linking with libgcc and libstdc++ and more

all: link

remake: clean all

image/%:	# if you ever need to link an image
	objcopy -I binary -O elf64-x86-64 obj/$*.obj build/$*.o

run:
	$(EXE)

clean:
	rm $(subst build/, build\, $(OBJ))

build/%.o: src/%.cpp
	g++ $(INCLUDE) -c src/$*.cpp -o build/$*.o $(DBG) $(EXTRA)

link: $(OBJ)
	g++ $(OBJ) -o $(EXE) $(LIB) $(STATIC) $(DBG) $(EXTRA)
