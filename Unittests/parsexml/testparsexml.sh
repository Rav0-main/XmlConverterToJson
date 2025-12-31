#!/bin/bash

wait_to_press_enter(){
	read -p "Press <Enter> to close .sh-script."
}

src_cpp_file="../../parsexml.cpp"
src_o_file="./parsexml.o"
test_cpp_files="./testparsexml.cpp ./assertion.cpp"
test_o_files="./testparsexml.o ./assertion.o"
exe_file="./testparsexml.exe"
SKIP_COMPILE_SRC="-so"
SKIP_COMPILE_TESTS="-to"

#if need compile src_cpp_file -> src_o_file
if [ ! \( "$1" = "$SKIP_COMPILE_SRC" -o "$2" = "$SKIP_COMPILE_SRC" \) ] ; then
	echo Compiling "\"$src_o_file\"" from "\"$src_cpp_file\"..."

	if g++ -c "$src_cpp_file" -I.. -std=c++20 -o "$src_o_file" ; then
		echo Success compiled object-file of source!
	
	else
		echo Compiling errors...
		wait_to_press_enter
		exit 1
	echo
	fi

else
	echo Skip compiling of "\"$src_cpp_file\"."
fi

#if need compile test_cpp_files -> test_o_files
if [ ! \( "$1" = "$SKIP_COMPILE_TESTS" -o "$2" = "$SKIP_COMPILE_TESTS" \) ] ; then
	echo Compiling $test_cpp_files...

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

if g++ -std=c++20 -o "$exe_file" "$src_o_file" $test_o_files ; then
	echo Success compiled "$exe_file";

else
	echo Compiling errors...
	wait_to_press_enter
	exit 1
fi

echo

echo TEST LOGS:
echo -------------------------------------------------------------

if "$exe_file" ; then
	echo "$exe_file" success completed!
else
	echo "$exe_file" completed with error!
fi

wait_to_press_enter

exit 0