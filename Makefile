all : ./libs/libminifb.a ./bin/window.o ./bin/framebuffer.o ./bin/draw.o libcfx.a ./bin/test_basic

LINK_FLAGS = -L./bin/ -L./bin/ -lX11 -llibminifb.a -lm
C_FLAGS = -I./include/
CC = cc

./bin/test_basic : ./tests/basic.c libcfx.a 
	$(CC) ./tests/basic.c $(C_FLAGS) -L./bin/ -lcfx -lX11 -lm -o ./bin/test_basic 
	
libcfx.a : ./bin/window.o ./bin/framebuffer.o ./bin/draw.o
	./build_archive.sh

./bin/framebuffer.o : ./src/framebuffer.c ./libs/libminifb.a
	$(CC) -c ./src/framebuffer.c $(C_FLAGS) $(LINK_FLAGS) -o ./bin/framebuffer.o

./bin/window.o : ./src/window.c ./libs/libminifb.a
	$(CC) -c ./src/window.c $(C_FLAGS) $(LINK_FLAGS) -o ./bin/window.o

./bin/draw.o : ./src/draw.c ./libs/libminifb.a
	$(CC) -c ./src/draw.c $(C_FLAGS) $(LINK_FLAGS) -o ./bin/draw.o

./libs/libminifb.a :
	./build_deps.sh

deep_clean : clean
	./build_deps.sh -r

clean : 
	rm ./bin/*
