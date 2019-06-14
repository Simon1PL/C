#include <unistd.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc

int main(){
    int fd[2];
    pipe(fd);
    pid_t child=fork();
    if(child==0){
        close(fd[1]);
        char dane[11];
        sleep(1);
        if(read(fd[0], dane, 11)!=11)
            printf("abababa");
        printf("%s\n", dane);
    }
    else{
        close(fd[0]);
        write(fd[1], "ala ma kota\n", 12);
    }
    return 0;
}