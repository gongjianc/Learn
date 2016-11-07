#!/bin/bash

echo "This script ran at $(date +%b%d,%T)" > at.out
echo >> at.out
sleep 5
echo "This is the script's end" >> at.out
