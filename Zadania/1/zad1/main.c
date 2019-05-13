//functions for dll library
#include <stdio.h>
#include <dlfcn.h>

#ifndef DLL
//DLL library doesn't need this header file
#include "library.h"
#endif

int main() {
#ifdef DLL
//open library
//function returns library handler
//takes dynamic library path and a flag
void *handle = dlopen("./liblibrary.so", RTLD_LAZY);

//now - pointers to all used functions
//function dlsym takes library handler and function name, returns function pointer
char** (*create_table)(int) = dlsym(handle, "create_table");
int (*search_directory)(char*, char*, char*) = dlsym(handle, "search_directory");
void (*remove_block)(int) = dlsym(handle, "remove_block");
//ok, we can use functions from our DLL library
#endif

	create_table(10);
	printf("indeks:%d\n\n", search_directory(".", "l*", "wynik.txt"));
	remove_block(0);

#ifdef DLL
  //close library after doing all stuff
  dlclose(handle);
#endif
}
