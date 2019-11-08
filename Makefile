SOURCES =  ./src/stb_image.c ./src/window.c ./src/framebuffer.c ./src/draw.c ./src/texture.c ./src/text.c
OBJECTS =  ./bin/stb_image.o ./bin/window.o ./bin/framebuffer.o ./bin/draw.o ./bin/texture.o ./bin/text.o

all : ./libs/libminifb.a  $(OBJECTS) libcfx.a ./bin/test_basic 

LINK_FLAGS = -L./bin/ -L./bin/ -lX11 -llibminifb.a -lm
C_FLAGS = -I./include/
CC = cc

./bin/test_basic : ./tests/basic.c libcfx.a 
	$(CC) ./tests/basic.c $(C_FLAGS) -L./bin/ -lcfx -lX11 -lm -o ./bin/test_basic 
	
libcfx.a : $(OBJECTS)
	./build_archive.sh

./bin/%.o : ./src/%.c 
	$(CC) -c $< -o $@ $(C_FLAGS) $(LINK_FLAGS)

$(OBJECTS) : $(SOURCES)

./libs/libminifb.a :
	./build_deps.sh

deep_clean : clean
	./build_deps.sh -r

clean : 
	rm ./bin/*