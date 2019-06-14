all: clear
	gcc -std=c99 server.c -o server -lpthread
	gcc -std=c99 client.c -o client -lpthread

clear:
	rm -f server
	rm -f client
