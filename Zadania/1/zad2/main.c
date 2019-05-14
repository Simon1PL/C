#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>

#include "../zad1/library.h"

char** results=NULL;
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
	if (!strcmp(tasks[1], "report")) { make_raport(); return 1; }
	if (!atoi(tasks[1])) {
		printf ("niepoprawny rozmiar\n");
		help();
		return-1;
	}
	rozmiar=atoi(tasks[1]);
	results=create_table(atoi(tasks[1]));
	int i=2;
	while (i<argc-1) {
		printf("%s:\n", tasks[i]);
		i+=parse_tasks(tasks, i);
	}
	print_results();
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
	for (i=0; i<rozmiar; i++){
		if (results[i]!=NULL) printf("%s\n", results[i]);
	}
}

void make_raport() {
	FILE* report=fopen("raport.txt", "a");
	if (report) {
		fprintf(report, "		real_time:	sys_time:	user_time:\n");
		fclose(report);	
	}
	my_clock=reset_time(my_clock);
	create_table(50000);
	print_clock("creatTab(50000)", "raport.txt", my_clock);
	my_clock=reset_time(my_clock);
	search_directory(".", "an*", "results.txt");
	print_clock("search(small)", "raport.txt", my_clock);
	my_clock=reset_time(my_clock);
	remove_block(0);
	search_directory("/home/students/s/z:~/SysOpy", "an*", "results.txt");
	print_clock("search(medium)", "raport.txt", my_clock);
	my_clock=reset_time(my_clock);
	remove_block(0);
	search_directory("/home/students/s/z:~", "an*", "results.txt");
	print_clock("search(big)", "raport.txt", my_clock);
	my_clock=reset_time(my_clock);
	remove_block(0);
	search_directory("/home/students/s/z:~", "ana*", "results.txt");
	print_clock("save(small)", "raport.txt", my_clock);
	my_clock=reset_time(my_clock);
	remove_block(0);
	search_directory("/home/students/s/z:~", "an*", "results.txt");
	print_clock("save(medium)", "raport.txt", my_clock);
	my_clock=reset_time(my_clock);
	remove_block(0);
	search_directory("/home/students/s/z:~", "a*", "results.txt");
	print_clock("save(big)", "raport.txt", my_clock);
	my_clock=reset_time(my_clock);
	remove_block(0);
	int i;
	for (i=0; i<500; i++) {
		search_directory(".", "m*", "results.txt");
		remove_block(0);	
	}
	print_clock("sea&rem(*500)", "raport.txt", my_clock);
	my_clock=reset_time(my_clock);
	system("rm results.txt");
	//system("clear");
}

struct Times* reset_time(struct Times* clock) {
	if(clock!=NULL) {
		free(clock);
		clock=NULL;
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
}
