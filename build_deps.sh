#!/bin/sh

if [ $# -gt 0 ] && [ $1 = "-r" ]
then
	echo "Cleaning build stuff"
	rm ./libs/libminifb.a
	cd ./modules/minifb
	rm ./build -r
	
else
	echo "===Building minifb==="
	cd ./modules/minifb
	mkdir ./build/
	cd ./build/
	cmake .. -DUSE_WAYLAND_API=OFF
	make  
	cp libminifb.a ../../../libs/libminifb.a
fi

	
