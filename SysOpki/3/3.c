#include <unistd.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc
#include <sys/resource.h>
#include <sys/types.h>

int main(){
    pid_t myPid=getpid();
    pid_t PPid=getppid();
    //printf("my pid: %d parent pid: %d\n", myPid, PPid);
    pid_t childPid=fork();
    if(childPid==0){
        //child (proces potomny)
        execl("/bin/ls", "ls", "-l",null);
        _exit(2);
    }
    else{
        //parent
    }
    return 0;
}