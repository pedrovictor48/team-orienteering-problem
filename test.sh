#!bin/zsh

make
echo "" > out
for entry in ./instances/*.txt
do
	time ./run < $entry >> out
	echo "\n" >> out
done
