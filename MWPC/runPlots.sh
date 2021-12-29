#!/bin/sh

filename=$1
echo "Processing " $filename
rm -f in.dat
ln -s $filename in.dat
./WCConverter
root -l 'Plots.C'
