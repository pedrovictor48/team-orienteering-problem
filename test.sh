#!/bin/bash

rm ./out
for entry in ./instances/*.txt
do
	./run $entry
done
