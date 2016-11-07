#!/bin/bash

function testit {
    local newarray
    newarray=$(echo "$@")
    echo "the new array value is: ${newarray[*]}"
}

myarray=(1 2 3 4 5)
echo "The original array is ${myarray[*]}"
testit ${myarray[*]}

