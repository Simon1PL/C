static:
	#clean up before you start
	make clear
	#first, .o files
	gcc -Wall -c lib1.c
	gcc -Wall -c lib2.c
	#this command creates library:
		#argument lib<libraryName>.a means library file
		#next - all .o files
	ar crs liblibrary.a lib1.o lib2.o
	#let's compile main file with our library:
		#-L<path> option imforms in which directory compiler should search for libarary file
		#-l<libraryName> option is about added librarywhich library to add
	gcc -Wall main.c -o main -L. -llibrary
	#clean up before you finish
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


clear:
	rm -f *.o *.a *.so main
clean: 
	make clear
	rm wynik.txt
