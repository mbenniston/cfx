all : ./libs/libminifb.a ./bin/window.o libcfx.a

libcfx.a : ./bin/window.o
	./build_archive.sh

./bin/window.o : ./src/window.c ./libs/libminifb.a
	gcc -c ./src/window.c -I./include/ -L./libs/ -llibminifb.a -lX11 -o ./bin/window.o

./libs/libminifb.a :
	./build_deps.sh

deep_clean : clean
	./build_deps.sh -r

clean : 
	rm ./bin/*
