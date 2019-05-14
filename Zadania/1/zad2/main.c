#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>

#include "../zad1/library.h"

char** wyniki=NULL;
int rozmiar;

void make_raport();
void help();
int parse_tasks(char** tasks, int i);
void print_results();

struct Times {
	clock_t real_time;
	clock_t sys_time;
	clock_t user_time;
};

struct Times* reset_time(struct Times* clock);
struct Times* set_clock(struct Times* clock);
void print_clock(const char * time_description, const char* report_file, struct Times* clock);

struct Times* my_clock=NULL;

int main(int argc, char ** tasks) {
	if (!tasks[1]) { help(); return-1; }
	if (!strcmp(tasks[1], "report")) {
		make_raport(); 
		print_results();
		return 0;
	}
	if (!atoi(tasks[1])) {
		printf ("niepoprawny rozmiar\n");
		help();
		return-1;
	}
	rozmiar=atoi(tasks[1]);
	wyniki=create_table(atoi(tasks[1]));
	int i=2;
	while (i<argc-1) {
		printf("%s:\n", tasks[i]);
		i+=parse_tasks(tasks, i);
	}
	print_results();
	free(wyniki);
	return 0;
}

void help() {
	printf("use: ./main size search_directory directory file tmp.txt remove_block index\nnp. ./main 1 search_directory . \"m*\" tmp.txt remove_block 0\n");
}

int parse_tasks(char** tasks, int i) {
	if (!strcmp(tasks[i], "search_directory")) {
		if (!strcmp(tasks[i+3], "main.c") || !strcmp(tasks[i+3], "Makefile")) {
			printf("permission denied, use (\") before and after file name\n");
			return 4;
		}
		search_directory(tasks[i+1], tasks[i+2], tasks[i+3]);
		return 4;
	}
	else if (!strcmp(tasks[i], "remove_block")) {
		if (atoi(tasks[i+1])) {
			printf ("niepoprawny index\n");
			help();
			return 1;
		}
		remove_block(atoi(tasks[i+1]));
		return 2;
	}
	else if (!strcmp(tasks[i], "help")) {
		help();
		return 1;
	}
	else {
		printf("nie ma komendy %s\n", tasks[i]);
		help();
		return 1;
	}
}

void print_results(){
	printf("results:\n");
	int i;
	for (i=0; i<5000; i++){
		if (wyniki[i]!=NULL) printf("%s\n", wyniki[i]);
	}
}

void make_raport() {
	char* tmp="results.txt";
	char* raport="report.txt";
	FILE* report=fopen(raport, "a");
	if (report) {
		fprintf(report, "		real_time:	sys_time:	user_time:\n");
		fclose(report);	
	}
	rozmiar=50000-1;
	my_clock=reset_time(my_clock);
	wyniki=create_table(rozmiar+1);
	print_clock("creatTab(50000)", raport, my_clock);
	//SMALL
	search_directory(".", "*.c", tmp);
	print_clock("search(small)", raport, my_clock);
	zapiszWynik(tmp);
	print_clock("save(small)", raport, my_clock);
	remove_block(1);
	print_clock("remove(small)", raport, my_clock);
	//MEDIUM
	search_directory("~/SysOpy", "*.c", tmp);
	print_clock("search(medium)", raport, my_clock);
	zapiszWynik(tmp);
	print_clock("save(medium)", raport, my_clock);
	remove_block(2);
	print_clock("remove(medium)", raport, my_clock);
	//BIG
	search_directory("~", "*.c", tmp);
	print_clock("search(big)", raport, my_clock);
	zapiszWynik(tmp);
	print_clock("save(big)", raport, my_clock);
	remove_block(3);
	print_clock("remove(big)", raport, my_clock);
	int i;
	for (i=0; i<500; i++) {
		search_directory(".", "*.c", tmp);
		remove_block(3);	
	}
	print_clock("sea&rem(*500)", raport, my_clock);
	system("rm results.txt");
	system("clear");
}

struct Times* reset_time(struct Times* clock) {
	if(clock!=NULL) {
		clock=NULL;
		free(clock);
	}
	return set_clock(clock);
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

void print_clock(const char * time_description, const char* report_file, struct Times* clock) {
	if (clock==NULL) { printf("ERROR!@!"); return; }
	struct Times* current_time=NULL;
	current_time=reset_time(current_time); 
	double real_time=(double)(current_time->real_time-clock->real_time)/sysconf(_SC_CLK_TCK);
	double sys_time=(double)(current_time->sys_time-clock->sys_time)/sysconf(_SC_CLK_TCK);
	double user_time=(double)(current_time->user_time-clock->user_time)/sysconf(_SC_CLK_TCK);
	FILE* report=fopen(report_file, "a");
	if (report) {
		fprintf(report, "%s	%lf 	%lf	%lf\n", time_description, real_time, sys_time, user_time);
		fclose(report);	
	}
	clock=reset_time(clock);
}
