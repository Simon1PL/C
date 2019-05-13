all:
	make -s static

static:
	make -s clear
	gcc -c library.c
	ar rcs liblibrary.a library.o
	gcc -c 1.c
	gcc 1.o liblibrary.a -o main
	rm -f *.o
	./main

shared:
	make -s clear
	gcc -c -fPIC library.c
	gcc -shared -o liblibrary.so library.o
	gcc 1.c -o main -L. -llibrary -Wl,-rpath=`pwd`
	rm -f *.o
	./main

dll:
	make -s clear
	gcc -c -fPIC library.c
	gcc -shared -o liblibrary.so library.o
	gcc 1.c -o main -L. -ldl -D DLL
	rm -f *.o
	./main

clear:
	rm -f *.o *.a *.so main