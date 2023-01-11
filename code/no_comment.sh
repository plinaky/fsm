#!/bin/bash

for f in /home/vincent/projects/proj4188_videocameraservice/src/**/*.cpp
do
  echo $f
  ./no_comment $f
done
