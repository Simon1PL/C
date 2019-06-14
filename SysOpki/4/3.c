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
    struck sigaction sig;
    act.sa_handler=obsluga;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    siagaction(SIGINT, &act, NULL);
    while(1){}
    return 0;
}