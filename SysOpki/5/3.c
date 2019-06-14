#include <unistd.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc

int main(){
    int fd[2];
    pipe(fd);
    pid_t child=fork();
    if(child==0){
        close(fd[1]);
        char dane[12];
        read(fd[0], dane, 12);
        printf("%s", dane);
    }
    else{
        close(fd[0]);
        write(fd[1], "ala ma kota\n", 12);
    }
    pid_t pid = fork();
    if (pid == 0) { // dziecko
        close(fd[1]); 
        dup2(fd[0], STDIN_FILENO);
    }
    else { // rodzic
        write(fd[1],"makota",6);
    }
    return 0;
}