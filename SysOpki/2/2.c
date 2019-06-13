#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc


int main(){
    //creat("plik.txt", O_RDWR);
    int plik=open("plik.txt", O_RDWR | O_APPEND | O_CREAT | O_TRUNC);
    write(plik, "Ala ma kota\n", 12);ss
    char dane[11];
    lseek(plik, 0, SEEK_SET);
    if(read(plik, &dane, 11)!=11)
        printf("blad odczytu %d\n", a);
    close(plik);
    return 0;
}