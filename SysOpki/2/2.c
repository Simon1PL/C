#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc


int main(){
    //creat("plik.txt", O_RDWR);
    int plik=open("plik.txt", O_RDWR | O_APPEND | O_CREAT | O_TRUNC);
    write(plik, "Ala ma kota\n", 12);
    char dane[12];
    int a=read(plik, &dane, 11);
    if(a!=11)
        printf("blad odczytu %d", a);
    close(plik);
    return 0;
}