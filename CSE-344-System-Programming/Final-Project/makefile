client:
	rm -f client
	gcc client.c -pthread -o client -lm

server:
	gcc server.c -pthread -o server -lm

runClient:
	./client 192.168.1.101 8091 1111 33 44

runServer:
	./server 192.168.1.101 8091 111 46 324657

clean:
	rm -f client server