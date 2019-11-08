#!/bin/sh

#Extracts object files from minifb and places them in new archive with cfx, reduces linking dependencies to just 1 -> X11

cd ./bin/
ar -x ../libs/libminifb.a
ar -qc libcfx.a *.o 
