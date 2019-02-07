#!/bin/bash
if [ ! -d "dist" ]
then
	echo "Creating 'dist' folder..."
	echo
	mkdir dist
fi
echo "Files to be compiled:"
CPPS=
DIR=
function list_files()
{
	if !(test -d "$1") 
	then return
	fi
	cd "$1"
	if [ -z "$DIR" ]
	then
		if [ $1 != '.' ]
		then DIR=$1
		fi
	else DIR="$DIR/$1"
	fi
	for i in *
	do
		if test -d "$i"
		then 
			list_files "$i"
			cd ..
			DIR=$(dirname $DIR)
			if [ $DIR == '.' ]
			then DIR=
			fi
		elif [ ${i##*.} == "cpp" ]
		then
			NEW_CPP=
			if [ -z "$DIR" ]
			then NEW_CPP="$i"
			else NEW_CPP="$DIR/$i"
			fi
			echo "    $NEW_CPP"
			CPPS="$CPPS $NEW_CPP"
		fi
	done
}
if [ $# -eq 0 ]
then list_files .
else
	for i in $*
	do
		DIR="$1"
		list_files "$DIR"
		shift 1
	done
fi
echo
echo "Start compilation..."
g++ -std=c++11 $CPPS -o dist/app
CODE=$?
if [ $CODE != 0 ]
then
	echo "Compilation FAIL with status $CODE"
	exit $CODE
fi
echo "Compilation SUCCESSFUL"
exit 0
