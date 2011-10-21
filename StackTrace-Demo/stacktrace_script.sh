#!/bin/bash

echo "Enter the file name you wish to profile ? ";

read inputline
what="$inputline"
gcc -g -finstrument-functions instrument.c $what -o output 
./output
stacktrace output 
stacktrace output $what 
gcc -g -finstrument-functions instrument.c $what -o output 
./output
stacktrace output stack
dot -Tjpg graph.dot -o graph.jpg
xdg-open graph.jpg
