#!/bin/bash

filesdir=$1
searchstr=$2

if [ $# -lt 2 ]
then
	echo "Not enough parameters"
	exit 1	
fi

mkdir -p "$(dirname "$1")" && touch  "$1"
echo "$searchstr" | cat > "$1"
