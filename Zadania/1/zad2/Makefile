static:
	make clear
	gcc -Wall main.c -o main -L../zad1 -llibrary
	rm -f *.o

shared:
	make clear
	gcc main.c -Wall -o main -L../zad1 -llibrary -Wl,-rpath=../zad1
	rm -f *.o

clear:
	rm -f *.o *.a *.so main

clean: 
	make clear
	rm report.txt
