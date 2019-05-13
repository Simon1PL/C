#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <time.h>
#include <sys/times.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

struct Times {
	clock_t real_time;
	clock_t sys_time;
	clock_t user_time;
};

void help() {
	printf("use: \n./main generate plik 100 512\n");
	printf("./main copy plik1 plik2 100 512 sys/lib\n");
	printf("./main sort plik 100 512 sys/lib\n");
}

struct Times* set_clock(struct Times * clock) {
	struct tms tms;
	if(clock==NULL){
		clock=calloc(1, sizeof(struct Times));
	}
	clock->real_time=times(&tms);
	clock->sys_time=tms.tms_stime;
	clock->user_time=tms.tms_utime;
	return clock;
}

struct Times* reset_time(struct Times* clock) {
	if(clock!=NULL) {
		free(clock);
		clock=NULL;
	}
	return set_clock(clock);
}

void print_clock(const char * time_description, const char* report_file, struct Times* clock) {
	if (clock==NULL) { printf("ERROR!@!"); return; }
	struct Times* current_time=NULL;
	current_time=reset_time(current_time); 
	double real_time=(double)(current_time->real_time-clock->real_time)/sysconf(_SC_CLK_TCK);
	double sys_time=(double)(current_time->sys_time-clock->sys_time)/sysconf(_SC_CLK_TCK);
	double user_time=(double)(current_time->user_time-clock->user_time)/sysconf(_SC_CLK_TCK);
	FILE* report=fopen(report_file, "a");
	if (report) {
		fprintf(report, "%s %g %g %g\n", time_description, real_time, sys_time, user_time);
		fclose(report);	
	}
}

char generateRecord() {
    return "ABCDEDFGHIJKLMNOPRSTUVWXYZ1234567890"[rand() % 36];
}

void generate(char **arguments) {
	int size = atoi(arguments[4]);
	int ammount = atoi(arguments[3]);
    FILE *file = fopen(arguments[2], "w");
    for (int i = 0; i < ammount; i++) {
        char buf[size];
        for (int j = 0; j <size; j++) {
            buf[j] = generateRecord();
        }
        //buff[len] = '\0';
        fwrite(buf, size, 1, file);
    }
    fclose(file);
}

void sortSys(char **arguments) {
	int size = atoi(arguments[4]);
	int ammount = atoi(arguments[3]);
    unsigned char *record = malloc((size) * sizeof(char));
    unsigned char *record1 = malloc((size) * sizeof(char));
	int file = open(arguments[2], O_RDWR);
	if (file > 0) {
		for (int i = 0; i < ammount-1; i++) {
			int min = i;
			lseek(file, size * i, SEEK_SET);
			read(file, record, size);
			for (int j = i + 1; j < ammount; j++) {
				lseek(file, size * j, SEEK_SET);
				read(file, record1, size);
				if (record[0] > record1[0]) {
					min = j;
					lseek(file, size * j, SEEK_SET);
					read(file, record, size);
				}
			}
			if (min != i) {
				lseek(file, size * i, SEEK_SET);
				read(file, record1, size);
				lseek(file, size * min, SEEK_SET);
				write(file, record1, size);
				lseek(file, size * i, SEEK_SET);
				write(file, record, size);
			}
		}
		close(file);
    }
	free(record);
	free(record1);
}

void sortLib(char **arguments) {
	int size = atoi(arguments[4]);
	int ammount = atoi(arguments[3]);
	char *record = malloc((size) * sizeof(char));
	char *record1 = malloc((size) * sizeof(char));
	FILE *file = fopen(arguments[2], "r+");
	if (file != NULL) {
		for (int i = 0; i < ammount-1; i++) {
			int min = i;
			fseek(file, size * i, 0);
			fread(record, size, 1, file);
			for (int j = i + 1; j < ammount; j++) {
				fseek(file, size * j, 0);
				fread(record1, size, 1, file);
				if ((unsigned char) record[0] > (unsigned char) record1[0]) {
					min = j;
					fseek(file, size * j, 0);
					fread(record, size, 1, file);
				}
			}
			if (min != i) {
				fseek(file, size * i, 0);
				fread(record1, size, 1, file);
				fseek(file, size*min, 0);
				fwrite(record1, size, 1, file);
				fseek(file, size * i, 0);
				fwrite(record, size, 1, file);
			}
		}
		fclose(file);
	}
	free(record);
	free(record1);
}

void copySys(char **arguments) {
	int size = atoi(arguments[5]);
	int ammount = atoi(arguments[4]);
	char *buffor = malloc(size * sizeof(char));
	int file = open(arguments[2], O_RDONLY);
	int newFile = open(arguments[3], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	for (int i = 0; i < ammount; i++) {
		read(file, buffor, size * sizeof(char));
		write(newFile, buffor, size * sizeof(char));
	}
	free(buffor);
	close(file);
	close(newFile);
}

void copyLib(char **arguments) {
	int size = atoi(arguments[5]);
	int ammount = atoi(arguments[4]);
	char *buffor = malloc(size * sizeof(char));
	FILE * file = fopen(arguments[2], "r");
	FILE* newFile = fopen(arguments[3], "w");
	for(int i = 0; i < ammount; i++){
		fread(buffor, size, 1, file);
		fwrite(buffor, size, 1, newFile);
	}
	fclose(file);
	fclose(newFile);
	free(buffor);
} 

void pomiary(){
	struct Times* my_clock=NULL;
	FILE* report=fopen("wyniki.txt", "w");
	if (report) {
		fprintf(report, "	real_time: sys_time: user_time: \n");
		fclose(report);	
	}
	char* tab[]={"1","4","512","1024","4096","8192"};
	char* arg[7];
	char* timeDescription=malloc(20);
	for (int i=0; i<6; i++){
		arg[2]="test.txt";
		arg[3]="1000";
		arg[4]=tab[i];
		strcpy(timeDescription, "generate");
		strcat(timeDescription,tab[i]);
		my_clock=reset_time(my_clock);
		generate(arg);
		print_clock(timeDescription, "wyniki.txt", my_clock);
		arg[3]="testCopy.txt";
		arg[4]="1000";
		arg[5]=tab[i];
		arg[6]="lib";
		strcpy(timeDescription, "copyLib");
		strcat(timeDescription,tab[i]);
		my_clock=reset_time(my_clock);
		copyLib(arg);
		print_clock(timeDescription, "wyniki.txt", my_clock);
		arg[2]="testCopy.txt";
		arg[3]=arg[4];
		arg[4]=arg[5];
		arg[5]=arg[6];
		strcpy(timeDescription, "sortLib");
		strcat(timeDescription,tab[i]);
		my_clock=reset_time(my_clock);
		sortLib(arg);
		print_clock(timeDescription, "wyniki.txt", my_clock);
		arg[5]=arg[4];
		arg[4]=arg[3];
		arg[3]="testCopy1.txt";
		arg[2]="test.txt";
		arg[6]="sys";
		strcpy(timeDescription, "copySys");
		strcat(timeDescription,tab[i]);
		my_clock=reset_time(my_clock);
		copySys(arg);
		print_clock(timeDescription, "wyniki.txt", my_clock);
		arg[2]="testCopy1.txt";
		arg[3]=arg[4];
		arg[4]=arg[5];
		arg[5]=arg[6];
		strcpy(timeDescription, "sortSys");
		strcat(timeDescription,tab[i]);
		my_clock=reset_time(my_clock);
		sortSys(arg);
		print_clock(timeDescription, "wyniki.txt", my_clock);
		//system("rm test.txt");
		//system("clear");
	}
}

void parse_tasks(char** tasks, int argc) {
	if (!strcmp(tasks[1], "generate")) {
		if(argc != 5) {
			printf("komenda generate przyjmuje 3 argumenty");
			help();
			return;
		}
		if (!atoi(tasks[3])||!atoi(tasks[4])) {
			printf ("argument 2 i 3 funkcji generate musi byc liczba\n");
			help();
			return;
		}
		generate(tasks);
		return;
	}
	else if (!strcmp(tasks[1], "sort")) {
		if(argc != 6) {
			printf("komenda sort przyjmuje 4 argumenty");
			help();
			return;
		}
		if (!atoi(tasks[3])||!atoi(tasks[4])) {
			printf ("argument 2 i 3 funkcji sort musi byc liczba\n");
			help();
			return;
		}
		if (!strcmp(tasks[5], "lib")) {
			sortLib(tasks);
			return;
		}
		if (!strcmp(tasks[5], "sys")) {
			sortSys(tasks);
			return;
		}
		printf ("argument 4 funkcji sort musi byc \"sys\" lub \"lib\"\n");
		help();
		return;
	}
	else if (!strcmp(tasks[1], "copy")) {
		if(argc != 7) {
			printf("komenda copy przyjmuje 5 argumentow");
			help();
			return;
		}
		if (!atoi(tasks[4])||!atoi(tasks[5])) {
			printf ("argument 3 i 4 funkcji copy musi byc liczba\n");
			help();
			return;
		}
		if (!strcmp(tasks[6], "lib")) {
			copyLib(tasks);
			return;
		}
		if (!strcmp(tasks[6], "sys")) {
			copySys(tasks);
			return;
		}
		printf ("argument 5 funkcji copy musi byc \"sys\" lub \"lib\"\n");
		help();
		return;
	}
	else if (!strcmp(tasks[1], "pomiary"))
		pomiary();
	else {
		printf("nie ma komendy %s\n", tasks[1]);
		help();
		return;
	}
}

int main(int argc , char**argv) {
	if(argc>1)
		parse_tasks(argv, argc);
	else help();
    return 0;
}
//scp C:\Users\Szymon\Downloads\main.c szielins@jabba.icsr.agh.edu.pl:~/main.c
//scp C:\Users\Szymon\Downloads\main.c szielins@jagular.iisg.agh.edu.pl:~/main.c