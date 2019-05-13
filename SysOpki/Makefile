all:
	make static
static:
	gcc -c library.c
	ar rcs liblibrary.a library.o
	gcc -c 1.c
	gcc 1.o liblibrary.a -o main
	./main
	rm -f *.o
dynamic:
	make clear
	#as with static library, we need .o files
	gcc -Wall -c lib1.c
	gcc -Wall -c lib2.c
	#now, cteate library.
		#-shared -fPIC - options for dynamic library
		#-o lib<libraryName>.so - our library file
		#then - .o files
	gcc -Wall -shared -fPIC -o liblibrary.so lib1.o lib2.o
	#compile everything together
		#I'm not sure, what "-Wl,-rpath=`pwd`" options do, but it's necessary :)
			#tkanks, krk_!
	gcc main.c -Wall -o main -L. -llibrary -Wl,-rpath=`pwd`
	#clean up!
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
clean: 
	make clear
	rm wynik.txt
