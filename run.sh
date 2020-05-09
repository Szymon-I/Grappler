#!/bin/bash
sources=""
for x in $(find ./$1/ -type f -name "*.cpp")
do
    sources=$sources" $x"
done

g++ -o "./$1/run.out" $sources -lglut -lGL -lGLU -lGLEW -lSOIL -I/usr/include/freetype2 -I/usr/include/libpng16 -L/usr/local/lib -lfreetype
cd $1
./run.out $2 $3