#!/bin/bash
sources=""
for x in $(find ./$1/ -type f -name "*.cpp")
do
    sources=$sources" $x"
done

g++ -o "./$1/run.out" $sources -lglut -lGL -lGLU -lGLEW -lSOIL
cd $1
./run.out $2 $3