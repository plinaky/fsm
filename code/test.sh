#!/bin/zsh

for f in $2/**/*.{h,hpp,c,cpp}
do
	echo $f
	$1 $f
done
