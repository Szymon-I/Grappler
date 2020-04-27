#!/bin/bash
cd $1
g++ -o "run.out" "main.cpp" -lglut -lGL -lGLEW
./run.out