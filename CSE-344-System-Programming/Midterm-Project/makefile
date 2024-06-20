all: client.c server.c
	gcc client.c -o client
	gcc server.c -o server -lrt -pthread
	./server dr 6

client: client.c
	gcc client.c -o client

server: server.c
	gcc server.c -o server -lrt -pthread

serverRun:
	./server dr 10

clean:
	rm -rf *o server client