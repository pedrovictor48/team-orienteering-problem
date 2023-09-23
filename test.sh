#!bin/zsh

make
echo "" > out
for entry in ./instances/*.txt
do
	./run < $entry >> out
done
