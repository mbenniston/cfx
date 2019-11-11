SOURCES =  ./src/stb_image.c ./src/window.c ./src/draw.c ./src/texture.c
OBJECTS =  ./bin/stb_image.o ./bin/window.o ./bin/draw.o ./bin/texture.o

all :  ./bin/test_basic ./bin/libcfx.a  $(OBJECTS) ./libs/libminifb.a

LINK_FLAGS = -g3
C_FLAGS = -I./include/ -Wall 
CC = cc

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

install : ./bin/libcfx.a
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 ./bin/libcfx.a $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/cfx
	install -m 644 ./include/*.h $(DESTDIR)$(PREFIX)/include/cfx/

uninstall : 
	rm  $(DESTDIR)$(PREFIX)/lib/libcfx.a
	rm -r $(DESTDIR)$(PREFIX)/include/cfx


leak_check : ./bin/test_basic
		valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./bin/test_basic

./bin/test_basic : ./tests/basic.c ./bin/libcfx.a
	$(CC) ./tests/basic.c $(C_FLAGS) -L./bin/ -lcfx -lX11 -lm -o ./bin/test_basic 
	
./bin/libcfx.a : $(OBJECTS) ./libs/libminifb.a
	./build_archive.sh

./bin/%.o : ./src/%.c 
	$(CC) -c $< -o $@ $(C_FLAGS) $(LINK_FLAGS)

./libs/libminifb.a :
	./build_deps.sh


deep_clean : clean
	./build_deps.sh -r

clean : 
	rm ./bin/*
