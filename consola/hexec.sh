#!/bin/bash
FILE=consola
make $FILE
if test -f "./$FILE"; then
    valgrind --tool=helgrind ./$FILE $1 $2
fi