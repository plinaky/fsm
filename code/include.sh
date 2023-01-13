#!/bin/zsh

for f in /usr/src/linux-headers-5.4.0-131/**/*.h
do
	echo $f
	./include $f
done
