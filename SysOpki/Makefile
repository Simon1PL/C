all:
	gcc -c library.c
	ar rcs liblibrary.a library.o
	gcc -c 1.c
	gcc 1.o liblibrary.a -o main
	./main