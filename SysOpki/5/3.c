#include <unistd.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc

int main(){
    int fd[2];
    pipe(fd);
    pid_t child=fork();
    if(child==0){
        close(fd[1]);
        char dane[20];
        sleep(1);
        read(fd[1], dane, 12);
        printf("%s\n", dane);
    }
    else{
        close(fd[0]);
        write(fd[0], "ala ma kota\n", 12);
    }
    return 0;
}