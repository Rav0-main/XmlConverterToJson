#!/bin/bash

wait_to_press_enter(){
	read -p "Press <Enter> to close .sh-script."
}

src_cpp_files="../../main.cpp ../../node.cpp ../../parsexml.cpp ../../tojson.cpp ../../help.cpp"
src_o_files="./main.o ./node.o ./parsexml.o ./tojson.o ./help.o"
exe_file="./fxmltjson.exe"
test_py_file="./testtime.py"
SKIP_COMPILE_SRC="-so"
SAVE_JSONs="-sj"

if [ ! \( "$1" = "$SKIP_COMPILE_SRC" -o "$2" = "$SKIP_COMPILE_SRC" \) ] ; then
	echo Compiling "\"$src_o_files\"" from "\"$src_cpp_files\"..."

	if g++ -I../../ -std=c++20 -c $src_cpp_files ; then
		echo Success compiled object-file of source!
	
	else
		echo Compiling errors...
		wait_to_press_enter
		exit 1
	echo
	fi

else
	echo Skip compiling of "\"$src_cpp_files\"."
fi

if g++ -std=c++20 -o "$exe_file" $src_o_files ; then
	echo Success compiled "\"$exe_file\"";

else
	echo Compiling errors...
	wait_to_press_enter
	exit 1
fi

echo

echo TIMER LOGS:
echo -------------------------------------------------------------

if python "$test_py_file" ; then
	echo "\"$test_py_file\"" success completed!

else
	echo "\"$test_py_file\"" ended with error!
	wait_to_press_enter
	exit 1
fi

if [ ! \( "$1" = "$SAVE_JSONs" -o "$2" = "$SAVE_JSONs" \) ] ; then
	rm -f *.json
	echo Removed all json-files.
fi

wait_to_press_enter

exit 0