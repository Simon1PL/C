all:
	make -s dll

static:
	make clear
	gcc -c library.c
	ar rcs liblibrary.a library.o
	gcc -c 1.c
	gcc 1.o liblibrary.a -o main
	rm -f *.o
	./main

shared:
	make clear
	gcc -c -fPIC library.c
	gcc -shared -o liblibrary.so library.o
	gcc 1.c -o main -L. -llibrary -Wl,-rpath=`pwd`
	rm -f *.o
	./main

dll:
	make clear
	gcc -c library.c
	gcc -shared -fPIC -o liblibrary.so library.o
	gcc -Wall main.c -o main -L. -ldl -D DLL
	rm -f *.o

clear:
	rm -f *.o *.a *.so main