all:
	make -s clear
	gcc main.c -o main -lpthread -lm
	rm -f *.o

test:
	make -s testwithecho

testwithecho:
	INPUT=balloons.pgm
	OUTPUT=out.pgm
	MODE=block
	echo -e "block test"

	echo -e "\nfilter1 (5x5)"
	FILTER=filter1
	echo -e "\n1 thread"
	./main 1 $(MODE)  $(INPUT)  $(FILTER)  $(OUTPUT)
	echo -e "\n2 threads"
	./main 2 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n4 threads"
	./main 4 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n8 threads"
	./main 8 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)

	echo -e "\n-----------------------------"
	echo -e "\nfilter2 (10x10)"
	FILTER=filter2
	echo -e "\n1 thread"
	./main 1 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n2 threads"
	./main 2 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n4 threads"
	./main 4 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n8 threads"
	./main 8 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)

	echo -e "\n-----------------------------"
	echo "-----------------------------"
	echo -e "intervaled test"
	MODE=intervaled

	echo -e "\nfilter1 (5x5)"
	FILTER=filter1
	echo -e "\n1 thread"
	./main 1 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n2 threads"
	./main 2 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n4 threads"
	./main 4 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n8 threads"
	./main 8 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)

	echo -e "\n-----------------------------"
	echo -e "\nfilter2 (10x10)"
	FILTER=filter2
	echo -e "\n1 thread"
	./main 1 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n2 threads"
	./main 2 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n4 threads"
	./main 4 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)
	echo -e "\n8 threads"
	./main 8 $(MODE) $(INPUT) $(FILTER) $(OUTPUT)

clear:
	rm -f *.o *.a *.so main