static:
	make -s static1
static1:
	#clean up before you start
	make clear
	#first, .o files
	gcc -c lib1.c
	#this command creates library:
		#argument lib<libraryName>.a means library file
		#next - all .o files
	ar crs liblibrary.a lib1.o
	#let's compile main file with our library:
		#-L<path> option imforms in which directory compiler should search for libarary file
		#-l<libraryName> option is about added library which library to add
	gcc main.c -o main -L. -llibrary
	#clean up before you finish
	rm -f *.o

shared:
	make clear
	#as with static library, we need .o files
	gcc -c -fPIC lib1.c
	#now, cteate library.
		#-shared -fPIC - options for shared library
		#-o lib<libraryName>.so - our library file
		#then - .o files
	gcc -shared -o liblibrary.so lib1.o
	#compile everything together
		#I'm not sure, what "-Wl,-rpath=`pwd`" options do, but it's necessary :)
			#tkanks, krk_!
	gcc main.c -o main -L. -llibrary -Wl,-rpath=`pwd`
	#clean up!
	rm -f *.o

dll:
	#Oh, three hours wasted... It was the main reason, why this tutorial was made. Enjoy!
	#clean up, .o files
	make clear
	gcc -c lib1.c
	#create dynamic library
	gcc -shared -fPIC -o liblibrary.so lib1.o
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