#!/bin/bash

if [ $# -gt 2 ]; then
	if [ "$1x" == "load" ]; then
	    echo "Loading module $2"
	fi
fi
module $*
/bin/bash
