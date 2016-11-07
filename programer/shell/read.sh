#!/bin/bash

if read -t 5 -p "Pleaase enter your name: " name
then
    echo "Hello $name, welcome to my script."
else
    echo
    echo "Sorry, too slow!"
fi

