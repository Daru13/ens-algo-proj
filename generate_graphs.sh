#!/bin/bash

# Graph generation constants
MIN_WEIGHT=0
MAX_WEIGHT=42

# Usage printing + exit
function printUsageAndExit()
{
	>&2 echo "Usage: $0 <nb vertices> <density> <nb graphs> <path>"
	exit 1
}

# Command line arguments
if [ $# -ne 4 ]; then
	printUsageAndExit
fi

ARG_nb_vertices=$1
ARG_density=$2
ARG_nb_graphs=$3
ARG_path=$4

# Generate a graph path for a given num
function getGraphPath() # num
{
	local base="/graph_"
	local fullpath="$ARG_path$base$1"

	echo "$fullpath"
}

# Generate a graph and print it to the desired output file
function genGraph() # num
{
	local num="$1"
	local path=$( getGraphPath "$num")

	# Graph generated using external program
	./build/generator "$ARG_nb_vertices" "$ARG_density" "$MIN_WEIGHT" "$MAX_WEIGHT" 1> "$path"
}

# Generate ARG_nb_graphs graphs
for ((i=0; i<ARG_nb_graphs; i++)); do
	genGraph "$i"
done
