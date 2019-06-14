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
        int fd1[2];
        pipe(fd1);
        pid_t pid = fork();
        if (pid == 0) { // dziecko
            close(fd1[1]); 
            int a=dup2(STDOUT_FILENO,fd1[0]);
            write(fd1[0],"makota",6);
        }
        else { // rodzic
            close(fd1[0]);
            write(fd1[1],"makotaa",6);
        }
    }
    return 0;
}