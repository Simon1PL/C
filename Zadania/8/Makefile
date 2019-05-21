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

test:
	make -s testwithecho

testwithecho:
	echo -e "block test"
	echo -e "\nfilter1 (5x5)"
	echo -e "\n1 thread"
	./main 1 $(MODE1) $(INPUT) $(FILTER1) $(OUTPUT)
	echo -e "\n2 threads"
	./main 2 $(MODE1) $(INPUT) $(FILTER1) $(OUTPUT)
	echo -e "\n4 threads"
	./main 4 $(MODE1) $(INPUT) $(FILTER1) $(OUTPUT)
	echo -e "\n8 threads"
	./main 8 $(MODE1) $(INPUT) $(FILTER1) $(OUTPUT)
	echo -e "\n-----------------------------"
	echo -e "\nfilter2 (20x20)"
	echo -e "\n1 thread"
	./main 1 $(MODE1) $(INPUT) $(FILTER2) $(OUTPUT)
	echo -e "\n2 threads"
	./main 2 $(MODE1) $(INPUT) $(FILTER2) $(OUTPUT)
	echo -e "\n4 threads"
	./main 4 $(MODE1) $(INPUT) $(FILTER2) $(OUTPUT)
	echo -e "\n8 threads"
	./main 8 $(MODE1) $(INPUT) $(FILTER2) $(OUTPUT)
	echo -e "\n-----------------------------"
	echo "-----------------------------"
	echo -e "intervaled test"
	echo -e "\nfilter1 (5x5)"
	echo -e "\n1 thread"
	./main 1 $(MODE2) $(INPUT) $(FILTER1) $(OUTPUT)
	echo -e "\n2 threads"
	./main 2 $(MODE2) $(INPUT) $(FILTER1) $(OUTPUT)
	echo -e "\n4 threads"
	./main 4 $(MODE2) $(INPUT) $(FILTER1) $(OUTPUT)
	echo -e "\n8 threads"
	./main 8 $(MODE2) $(INPUT) $(FILTER1) $(OUTPUT)
	echo -e "\n-----------------------------"
	echo -e "\nfilter2 (10x10)"
	echo -e "\n1 thread"
	./main 1 $(MODE2) $(INPUT) $(FILTER2) $(OUTPUT)
	echo -e "\n2 threads"
	./main 2 $(MODE2) $(INPUT) $(FILTER2) $(OUTPUT)
	echo -e "\n4 threads"
	./main 4 $(MODE2) $(INPUT) $(FILTER2) $(OUTPUT)
	echo -e "\n8 threads"
	./main 16 $(MODE2) $(INPUT) $(FILTER2) $(OUTPUT)

clear:
	rm -f *.o *.a *.so main