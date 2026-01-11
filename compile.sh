#!/bin/bash

wait_to_press_enter(){
	read -p "Press <Enter> to close .sh-script"
}

src_cpp_files="./main.cpp ./tag.cpp ./tojson.cpp ./parsexml.cpp ./help.cpp"
exe_file="fxmltjson.exe"

echo Compiling "\"$exe_file\""...

if g++ -O2 -I. -std=c++20 -o "$exe_file" $src_cpp_files ; then
	echo "\"$exe_file\"" success compiled!
	wait_to_press_enter
	exit 0

else
	echo Compiling errors...
	wait_to_press_enter
	exit 1
fi