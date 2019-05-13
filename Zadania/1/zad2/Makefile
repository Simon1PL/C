static:
	make clear
	gcc -Wall main.c -o main -L../zad1 -llibrary
	rm -f *.o

dynamic:
	#NIE DZIALA!!!
	make clear
	gcc main.c -Wall -o main -L../zad1 -llibrary -Wl,-rpath=`pwd`
	rm -f *.o

dll:
	#NIE DZIALA!!! jak wczytac zmienne globalne?
	make clear
	gcc -Wall main.c -o main -L. -ldl -D DLL
	rm -f *.o

clear:
	rm -f *.o *.a *.so main

clean: 
	make clear
	rm wynik.txt
