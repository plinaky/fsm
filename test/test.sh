#!/bin/zsh

for f in $2/**/*.$3
do
	$1 $f
done
