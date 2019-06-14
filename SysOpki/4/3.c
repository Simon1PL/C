#include <unistd.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void obsluga(int signal){
    printf("BOOM!\n");
}

void obsluga1(int signal, siginfo_t *dane, void *idk){
    printf("DANE: %d, %s\n", dane.si_int, dane.si_ptr);
}

int main(){
    //signal(SIGINT, obsluga);
    struct sigaction act;
    act.sa_handler=obsluga1;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    sigaction(SIGINT, &act, NULL);
    union sigval sig;
    sig.sival_int=1;
    sig.sival_ptr="ala ma kota";
    sigqueue(getpid(), SIGINT, sig); 
    //pause(); //czeka na sygnal
    return 0;
}