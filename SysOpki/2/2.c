#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc


int main(){
    //creat("plik.txt", O_RDWR);
    int plik=open("plik.txt", O_RDWR | O_APPEND | O_CREAT | O_TRUNC);
    int b=write(plik, "Ala ma kota\n", 12);
    printf("%d %d\n", plik, b);
    char dane[11];
    //lseek(plik, 0, SEEK_SET);
    int a=read(plik, &dane, 11);
    if(a!=11)
        printf("blad odczytu %d", a);
    lseek(plik, 0, SEEK_SET);
    close(plik);
    return 0;
}