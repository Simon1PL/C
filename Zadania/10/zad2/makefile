all: clear
	gcc -Wall -std=c99 server.c -o server -lpthread
	gcc -Wall -std=c99 client.c -o client -lpthread

clear:
	rm -f server
	rm -f client
