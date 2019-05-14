#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.h"

int size=0;
int free_blocks=0;
char** results=NULL;

char** create_table(int rozmiar) {
	results=(calloc(rozmiar, sizeof(char*)));
	int i;
	for (i = 0; i < 17000; i++)
	{
		printf("1:%d\n", results[i]);
		printf("1:%d\n", i);
	}
	size=rozmiar;
	free_blocks=rozmiar;
	return results;
}

int search_directory(char* dir, char* file, char* name_file_temp){
	if (free_blocks==0) {
		printf("brak miejsca na zapis wynikow\n");	
		return -1;	
	}
	char komenda[100];
	//sprintf(komenda, "find %s -name '%s'", dir, file);
	//system(komenda);
	sprintf(komenda,"find %s -name '%s' > %s", dir, file, name_file_temp);
	system(komenda);
	return (zapiszWynik(name_file_temp));	
}

int zapiszWynik(char* name_file_temp){
	FILE* wynik;
	int dlugosc;
	wynik=fopen(name_file_temp, "r");
	fseek (wynik, 0, SEEK_END);
	dlugosc=ftell (wynik);
	fseek (wynik, 0, SEEK_SET);
	if (dlugosc>0){
		char *result=calloc(1, dlugosc);
		char *wiersz=malloc(dlugosc);;
		while(fgets(wiersz, dlugosc, wynik)){
			strcat(result, wiersz);
		}
		free(wiersz);
		int i = 0;
		while(results[i]!=NULL) i++;
		results[i]=result;
		free_blocks--;
		fclose(wynik);
		return i;
	}
	else {
		printf("nie znaleziono zadnego pliku pasujacego do kryteriow\n");
		fclose(wynik);
		return -1;
	}
}

void remove_block(int index){
	if(index>size-1 || results[index]==NULL){
		printf("nie ma bloku o takim indeksie\n");	
		return;	
	}
	results[index]=NULL;
	free(results[index]);
	free_blocks++;	
}