#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc


int main(){
    //creat("./plik.txt", O_RDWR);
    int pfd;
    if ((pfd = open("testfile.txt", O_WRONLY | O_CREAT | O_EXCL,
    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
    {
    fprintf(stderr, "Cannot open /etc/ptmp. Try again later.\n");
    exit(1);
    }
    int filedesc = open("testfile.txt", O_WRONLY, O_APPEND);
    if(filedesc < 0){
    write(1,"THEREwas an error writing to testfile.txt\n", 43);
        return 1;
    }
 
    if(write(filedesc,"This will be output to testfile.txt\n", 36) != 36)
    {
        write(1,"There was an error writing to testfile.txt\n", 43);    // strictly not an error, it is allowable for fewer characters than requested to be written.
        return 1;
    }
    int plik=open("plik.txt", O_CREAT || O_RDWR);
    int ret=write(plik, "ala ma kota\n", 8);
    printf("%d\n", plik);
    close(plik);
    return 0;
}