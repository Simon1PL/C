#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> //printf
#include<stdlib.h> //alloc


int main(){
    //creat("./plik.txt", O_RDWR);
    int plik=open("./plik.txt", O_RDWR || O_CREAT);
    int ret=write(plik, "ala ma kota\n", 8);
    printf("%d\n", plik);
    close(plik);
    return 0;
}