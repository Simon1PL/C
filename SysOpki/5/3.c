#include <unistd.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc

int main(){
    int child=fork();
    int fd[2];
    pipe(fd);
    if(child==0){
        close(fd[1]);
        char dane[20];
        sleep(3);
        read(fd[0], dane, 12);
        printf("%s\n", dane);
    }
    else{
        waitpid(child, NULL, 0);
        //close(fd[0]);
       // write(fd[1], "ala ma kota\n", 12);
    }
    return 0;
}