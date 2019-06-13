#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> //printf
#include<stdlib.h> //alloc


int main(){
    creat("./plik.txt", O_RDWR);
    open("./plik.txt", O_RDWR || O_CREAT);
    return 0;
}