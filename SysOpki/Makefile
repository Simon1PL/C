all:
	make shared

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
	#Oh, three hours wasted... It was the main reason, why this tutorial was made. Enjoy!

	#clean up, .o files
	make clear
	gcc -Wall -c lib1.c
	gcc -Wall -c lib2.c
	#create dynamic library
	gcc -Wall -shared -fPIC -o liblibrary.so lib1.o lib2.o
	#and now is the magic. For DLL you need some extra code in main.c file, check it out.
		#-L. - we know it
		#-ldl - for dll library
		#-D DLL - in main.c we can see several "#ifdef DLL" commands.
		#-D option could be understood like: compiler add command "#define DLL" to our main.c file
	gcc -Wall main.c -o main -L. -ldl -D DLL
	#clean up
	rm -f *.o
	#Hurray!

clear:
	rm -f *.o *.a *.so main