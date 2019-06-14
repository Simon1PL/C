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
    int fd1[2];
    pipe(fd1);
    int a=dup2(fd1[0], STDOUT_FILENO);
    write(fd1[0], "dd\n", 3);
    pid_t pid = fork();
    if (pid == 0) { // dziecko
        close(fd1[1]); 
    }
    else { // rodzic
        close(fd1[0]);
        write(fd1[1],"makota",6);
        printf("SHIET");
    }
    return 0;
}