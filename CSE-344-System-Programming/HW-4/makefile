all: 200104004066_main.c
	rm -rf *o MWCp
	gcc 200104004066_main.c -o MWCp  -lrt -pthread
	./MWCp 1 100 ../testdir ../tocopy

compile: 200104004066_main.c
	gcc 200104004066_main.c -o MWCp  -lrt -pthread

run:
	./MWCp 100 100 ../testdir ../tocopy

clean:
	rm -rf *o MWCp