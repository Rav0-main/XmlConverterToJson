#!/bin/bash

wait_to_press_enter(){
	read -p "Press <Enter> to close .sh-script."
}

src_cpp_files="../../tojson.cpp ../../tag.cpp"
src_o_files="./tojson.o ./tag.o"
test_cpp_files="./converttojson.cpp"
test_o_files="./converttojson.o"
exe_file="./converttojson.exe"
test_py_file="./testtojson.py"
SKIP_COMPILE_SRC="-so"
SKIP_COMPILE_TESTS="-to"

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

if [ ! \( "$1" = "$SKIP_COMPILE_TESTS" -o "$2" = "$SKIP_COMPILE_TESTS" \) ] ; then
	echo Compiling "\"$test_cpp_files\""...

	if  g++ -I. -std=c++20 -c $test_cpp_files ; then
		echo Success compiled object-files of tests!

	else
		echo Compiling errors...
		wait_to_press_enter
		exit 1
	fi

else
	echo Skip compiling of "\"$test_cpp_files\""...
fi

if g++ -std=c++20 -o "$exe_file" $src_o_files $test_o_files ; then
	echo Success compiled "\"$exe_file\"";

else
	echo Compiling errors...
	wait_to_press_enter
	exit 1
fi

echo
echo CONVERTING LOGS:
echo -------------------------------------------------------------

if "$exe_file" ; then
	echo "\"$exe_file\"" success completed!

else
	echo "\"$exe_file\"" completed with error!
	wait_to_press_enter
	exit 1
fi

echo

echo TEST LOGS:
echo -------------------------------------------------------------

if python "$test_py_file" ; then
	echo "\"$test_py_file\"" success completed!

else
	echo "\"$test_py_file\"" ended with error!
	wait_to_press_enter
	exit 1
fi

wait_to_press_enter

exit 0