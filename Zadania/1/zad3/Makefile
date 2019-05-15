CO0=gcc -Wall -O0
COS=gcc -Wall -Os
CO2=gcc -Wall -O2
all:
	make -s static
	./main report static
	make -s staticO0
	./main report staticO0
	make -s staticO2
	./main report staticO2
	#make -s staticOs
	#./main report staticOs
	make -s shared
	./main report shared
	make -s sharedO0
	./main report sharedO0
	make -s sharedO2
	./main report sharedO2
	#make -s sharedOs
	#./main report sharedOs
	make -s dll
	./main report dll
	make -s dllO0
	./main report dllO0
	make -s dllO2
	./main report dllO2
	#make -s dllOs
	#./main report dllOs

static:
	make clear
	gcc -c ../zad1/lib1.c
	ar crs liblibrary.a lib1.o
	gcc main.c -o main -L. -llibrary
	rm -f *.o

shared:
	make clear
	gcc -c -fPIC ../zad1/lib1.c
	gcc -shared -o liblibrary.so lib1.o
	gcc main.c -o main -L. -llibrary -Wl,-rpath=`pwd`
	rm -f *.o

dll:
	make clear
	gcc -c -fPIC ../zad1/lib1.c
	gcc -shared -o liblibrary.so lib1.o
	gcc -Wall main.c -o main -L. -ldl -D DLL
	rm -f *.o

clear:
	rm -f *.o *.a *.so main

clean: 
	make clear
	rm report.txt

staticO0:
	make clear
	$(CO0) -c ../zad1/lib1.c
	ar crs liblibrary.a lib1.o
	$(CO0) main.c -o main -L. -llibrary
	rm -f *.o

sharedO0:
	make clear
	$(CO0) -c -fPIC ../zad1/lib1.c
	$(CO0) -shared -o liblibrary.so lib1.o
	$(CO0) main.c -o main -L. -llibrary -Wl,-rpath=`pwd`
	rm -f *.o

dllO0:
	make clear
	$(CO0) -c -fPIC ../zad1/lib1.c
	$(CO0) -shared -o liblibrary.so lib1.o
	$(CO0) main.c -o main -L. -ldl -D DLL
	rm -f *.o

staticO2:
	make clear
	$(CO2) -c ../zad1/lib1.c
	ar crs liblibrary.a lib1.o
	$(CO2) main.c -o main -L. -llibrary
	rm -f *.o

sharedO2:
	make clear
	$(CO2) -c -fPIC ../zad1/lib1.c
	$(CO2) -shared -o liblibrary.so lib1.o
	$(CO2) main.c -o main -L. -llibrary -Wl,-rpath=`pwd`
	rm -f *.o

dllO2:
	make clear
	$(CO2) -c -fPIC ../zad1/lib1.c
	$(CO2) -shared -o liblibrary.so lib1.o
	$(CO2) main.c -o main -L. -ldl -D DLL
	rm -f *.o

staticOs:
	make clear
	$(COs) -c ../zad1/lib1.c
	ar crs liblibrary.a lib1.o
	$(COs) main.c -o main -L. -llibrary
	rm -f *.o

sharedOs:
	make clear
	$(COs) -c -fPIC ../zad1/lib1.c
	$(COs) -shared -o liblibrary.so lib1.o
	$(COs) main.c -o main -L. -llibrary -Wl,-rpath=`pwd`
	rm -f *.o

dllOs:
	make clear
	$(COs) -c -fPIC ../zad1/lib1.c
	$(COs) -shared -o liblibrary.so lib1.o
	$(COs) main.c -o main -L. -ldl -D DLL
	rm -f *.o