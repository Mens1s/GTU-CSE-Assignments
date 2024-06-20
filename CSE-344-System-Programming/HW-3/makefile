all: main.c
	rm -rf *o main
	gcc main.c -o main  -lrt -pthread
	./main

compile: main.c
	gcc main.c -o main  -lrt -pthread

run:
	./main

clean:
	rm -rf *o main