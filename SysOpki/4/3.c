#include <unistd.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void obsluga(int signum){
    printf("BOOM!");
}

int main(){
    signal(SIGINT, obsluga);
    while(1){}
    return 0;
}