//git pull
//scp C:\Users\Szymon\Documents\GitHub\SysOpy\SysOpki\1.c szielins@jagular.iisg.agh.edu.pl:~/sys.c
//scp C:\Users\Szymon\Documents\GitHub\SysOpy\SysOpki\1.c szielins@jabba.icsr.agh.edu.pl:~/sys.c
#include<unistd.h> //sleep
#include <time.h> //nanosleep czas(timespec)
#include <stdio.h> //printf
#include<stdlib.h> //malloc
#include <sys/times.h> //czas(tms)
#include<stdlib.h> //alloc
#include <dlfcn.h> //dll
#include "library.h"

int main(){
    void *handle = dlopen("liblibrary.so", RTLD_LAZY);
    if(!handle){printf("error dll\n");}
    void (*try_library)(char*)= dlsym(handle,"try_library");
    if(dlerror() != NULL){printf("error dll 2\n");}
    
    sleep(0.005);//w sec

    struct timespec t1, t2;
    t1.tv_sec=0;
    t1.tv_nsec=100000000;
    t2.tv_sec=0;
    t2.tv_nsec=0;
    nanosleep(&t1, &t2);

    clock_getres(CLOCK_REALTIME, &t1);
    clockid_t clk;
    clock_gettime(clk, &t1); //niewygodne bo sec i nanosec osobno
    if (clock_settime(clk, &t2)!=0)
        printf("BLAD, brak dostepu?\n");
    
    struct tms buff, buff1;
    clock_t start, stop, tstart, tstop;
    start=clock();
    tstart=times(&buff);
    int i;
    for(i=0; i< 90000000; i++) {}
    sleep(1); //clock() nie dziala podczas sleepa
    stop = clock();
    tstop=times(&buff1);
    double wynik=(double)(stop-start)/CLOCKS_PER_SEC;
    double twynik=(double)(tstop-tstart)/sysconf(_SC_CLK_TCK);
    printf("czas liczony clockiem: %f\nczas liczony timesem: %g\n", wynik, twynik);
    double utime=(double)(buff1.tms_utime-buff.tms_utime)/sysconf(_SC_CLK_TCK);
    printf("utime: %g\n", utime);

    double **tab=(double**)calloc(10, sizeof(double*)); //sizeof(doule*)==sizeof(double)
    for (i = 0; i < 10; i++)
        tab[i]=(double*)calloc(5, sizeof(double));
    free(tab);
    int *a=malloc(4*sizeof(int));
    a=realloc(a, 5*sizeof(int));
    free(a);
    
    try_library("ALA");
    printf("%d\n", size);

    dlclose(handle);
    return 0;
}