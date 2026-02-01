#!/bin/bash

wait_to_press_enter(){
	read -p "Press <Enter> to close .sh-script"
}

src_cpp_files="src/main.cpp src/tag.cpp src/tojson.cpp src/parsexml.cpp src/help.cpp src/isnum.cpp"
include_dir="./include/"
exe_file="fxmltjson.exe"
compiler="g++"

echo Compiling "\"$exe_file\""...

if $compiler -O2 -I$include_dir -std=c++20 -o "$exe_file" $src_cpp_files ; then
	echo "\"$exe_file\"" success compiled!
	wait_to_press_enter
	exit 0

else
	echo Compiling errors...
	wait_to_press_enter
	exit 1
fi