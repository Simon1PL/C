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
	#export LD_LIBRARY_PATH=/home/students/s/z/szielins/SysOpy/SysOpki/library:$LD_LIBRARY_PATH
	gcc -c -fPIC library.c
	#gcc -shared -o liblibrary.so library.o
	#gcc -Wall -Werror -fpic –c library.c
	gcc -shared –Wl -soname liblibrary.so.1 \ -o liblibrary.so.1.0.1 library.o –lc
	#ln –s liblibrary.so.1.0.1 liblibrary.so.1
	#ln –s liblibrary.so.1 liblibrary.so
	#gcc 1.c -llibrary -L ./ -o main
	gcc 1.c -Wall -o main -L. -llibrary -Wl,-rpath=`pwd`
	rm -f *.o
	
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