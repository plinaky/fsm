#!/bin/zsh

for f in $2/**/*.h
do
	echo $f
	$1 $f
done
