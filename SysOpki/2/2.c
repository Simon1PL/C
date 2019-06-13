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
    if(read(plik, dane, 12)!=12)
        printf("blad odczytu %s", dane);
    close(plik);
    return 0;
}