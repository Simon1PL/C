INPUT=MonaLisa.pgm
OUTPUT=out.pgm
MODE1=block
FILTER2=filter2
FILTER1=filter1
MODE2=interleaved
all:
	make -s clear
	gcc main.c -o main -lpthread -lm
	rm -f *.o
	./main 8 interleaved MonaLisa.pgm filter1 out.pgm

test:
	make -s testwithecho

testwithecho:
	echo -e "block test" >> Times.txt
	echo -e "\nfilter1 (5x5)" >> Times.txt
	echo -e "\n1 thread" > Times.txt
	./main 1 $(MODE1) $(INPUT) $(FILTER1) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n2 threads" >> Times.txt
	./main 2 $(MODE1) $(INPUT) $(FILTER1) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n4 threads" >> Times.txt
	./main 4 $(MODE1) $(INPUT) $(FILTER1) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n8 threads" >> Times.txt
	./main 8 $(MODE1) $(INPUT) $(FILTER1) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n-----------------------------" >> Times.txt
	echo -e "\nfilter2 (20x20)" >> Times.txt
	echo -e "\n1 thread" >> Times.txt
	./main 1 $(MODE1) $(INPUT) $(FILTER2) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n2 threads" >> Times.txt
	./main 2 $(MODE1) $(INPUT) $(FILTER2) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n4 threads" >> Times.txt
	./main 4 $(MODE1) $(INPUT) $(FILTER2) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n8 threads" >> Times.txt
	./main 8 $(MODE1) $(INPUT) $(FILTER2) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n-----------------------------" >> Times.txt
	echo "-----------------------------" >> Times.txt
	echo -e "interleaved test" >> Times.txt
	echo -e "\nfilter1 (5x5)" >> Times.txt
	echo -e "\n1 thread" >> Times.txt
	./main 1 $(MODE2) $(INPUT) $(FILTER1) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n2 threads" >> Times.txt
	./main 2 $(MODE2) $(INPUT) $(FILTER1) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n4 threads" >> Times.txt
	./main 4 $(MODE2) $(INPUT) $(FILTER1) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n8 threads" >> Times.txt
	./main 8 $(MODE2) $(INPUT) $(FILTER1) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n-----------------------------" >> Times.txt
	echo -e "\nfilter2 (20x20)" >> Times.txt
	echo -e "\n1 thread" >> Times.txt
	./main 1 $(MODE2) $(INPUT) $(FILTER2) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n2 threads" >> Times.txt
	./main 2 $(MODE2) $(INPUT) $(FILTER2) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n4 threads" >> Times.txt
	./main 4 $(MODE2) $(INPUT) $(FILTER2) $(OUTPUT) >> Times.txt
	rm out.pgm
	echo -e "\n8 threads" >> Times.txt
	./main 8 $(MODE2) $(INPUT) $(FILTER2) $(OUTPUT) >> Times.txt

clear:
	rm -f *.o *.a *.so main

clean:
	make clear
	rm out.pgm