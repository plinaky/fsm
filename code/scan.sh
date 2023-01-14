#!/bin/zsh

for f in /usr/src/linux-headers-5.0.0-32/**/*.h
do
	echo $f
	./scan $f
done
