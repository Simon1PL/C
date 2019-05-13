CFLAGS= -c -Wall

all:
	make -s static

static:
	make -s clear
	gcc $(CFLAGS) library.c
	ar rcs liblibrary.a library.o
	gcc $(CFLAGS) 1.c
	gcc 1.o liblibrary.a -o main
	rm -f *.o
	./main

shared:
	make -s clear
	gcc $(CFLAGS) -fPIC library.c
	gcc -shared -o liblibrary.so library.o
	gcc 1.c -o main -L. -llibrary -Wl,-rpath=..
	rm -f *.o
	./main

dll:
	make -s clear
	gcc $(CFLAGS) -fPIC library.c
	gcc -shared -o liblibrary.so library.o
	gcc 1.c -o main -L. -ldl -D DLL
	rm -f *.o
	./main

clear:
	rm -f *.o *.a *.so main