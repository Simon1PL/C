#include <unistd.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc
#include <sys/resource.h>
#include <sys/types.h>

int main(){
    pid_t myPid=getpid();
    pid_t PPid=getppid();
    printf("my: %d parent: %d\n", myPid, PPid);
    return 0;
}