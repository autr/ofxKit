#!/bin/bash

export LC_ALL=C

for example in $( find examples/example-* -maxdepth 1 -type d | grep -v osx )
do

    echo -----------------------------------------------------------------
    echo building  $example

    #projectGenerator .
    make Debug -j2 -C "$example"
    ret=$?
    if [ $ret -ne 0 ]; then
        echo error compiling $example
        exit
    fi
    make Release -j2 -C "$example"
    ret=$?
    if [ $ret -ne 0 ]; then
        echo error compiling $example
        exit
    fi

    echo -----------------------------------------------------------------
    echo
done