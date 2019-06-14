#include <unistd.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void obsluga(int signum){
    printf("BOOM!\n");
    exit(1);
}

int main(){
    //signal(SIGINT, obsluga);
    struct sigaction act;
    act.sa_handler=obsluga;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    siagaction(SIGINT, &act, NULL);
    //sigqueue(getpid(), SIGINT, const union sigval value) 
    while(1){}
    return 0;
}