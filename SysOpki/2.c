#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc


int main(){
    int fd;
    fd = open("a.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    creat("plik.txt", O_RDWR);
    int plik=open("plik.txt", O_RDWR);
    int ret=write(plik, "ala ma kota\n", 8);
    printf("%d\n%d\n", fd, ret);
    close(plik);
    return 0;
}