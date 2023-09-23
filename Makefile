CPLEXFLAGS=-O3 -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -I/home/pedro/cplex/CPLEX_Studio129/cplex/include -I/home/pedro/cplex/CPLEX_Studio129/concert/include  -L/home/pedro/cplex/CPLEX_Studio129/cplex/lib/x86-64_linux/static_pic -lilocplex -lcplex -L/home/pedro/cplex/CPLEX_Studio129/concert/lib/x86-64_linux/static_pic -lconcert -lm -pthread -std=c++0x -ldl

CFLAGS=-std=c++11 -static-libstdc++ -static-libgcc -Wall

all:
	g++ main.cpp -o run $(CPLEXFLAGS) $(CFLAGS)

clean:
	rm -f *.o
