#!/bin/bash

filesdir=$1
searchstr=$2

if [ $# -lt 2 ]
then
	echo "Not enough parameters"
	exit 1
elif [ ! -d "$filesdir" ]
then
	echo "file not found"
	exit 1
fi

#X=$( grep -c -r "$searchstr" $filesdir | grep -E '[[:digit:]]+')
X=$( grep -lr "$searchstr" $filesdir | wc -l )
Y=$( grep -hr "$searchstr" $filesdir | wc -l )
echo  "The number of files are ${X} and the number of matching lines are ${Y}"
