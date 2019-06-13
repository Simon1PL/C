#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> //printf
#include<stdlib.h> //alloc


int main(){
    int plik=creat("./plik.txt", O_RDWR);
    //open("./plik.txt", O_RDWR || O_CREAT || O_APPEND);
    int ret=write(plik, "ala ma kota\n", 12);
    printf("%d\n", ret);
    return 0;
}