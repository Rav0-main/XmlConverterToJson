#!/bin/bash

wait_to_press_enter(){
	read -p "Press <Enter> to close .sh-script."
}

src_file="../../parsexml.cpp"
dist_file="./parsexml.o"
test_files="./testparsexml.cpp ./assertion.cpp"
exe_file="./testparsexml.exe"

#if not need compile src_file
if [ ! \( "$#" -gt 0 -a "$1" = "-so" \) ] ; then
	echo Compiling "\"$dist_file\"" from "\"$src_file\"..."

	if g++ -c "$src_file" -I.. -std=c++20 -o "$dist_file" ; then
		echo Success compiled object-file!
	
	else
		echo Compiling errors...
		wait_to_press_enter
		exit 1
	echo
	fi

else
	echo Skip compiling "\"$src_file\"."

fi

echo Compiling "\"$exe_file\""...
if g++ -I. -std=c++20 -o "$exe_file" "$dist_file" $test_files ; then
	echo Success compiled tests!

else
	echo Compiling errors...
	wait_to_press_enter
	exit 1
fi

echo

echo TEST LOGS:
echo -------------------------------------------------------------

"$exe_file"

wait_to_press_enter

exit 0