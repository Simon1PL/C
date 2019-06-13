#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <stdio.h> //printf
#include <stdlib.h> //alloc
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <ftw.h>

int files=0, dires=0;

char* formatdate(char* str, time_t val)
{
        strftime(str, 36, "%d.%m.%Y %H:%M:%S"/*"%c"*/, localtime(&val));
        return str;
}

int fnFiles(const char *filepath, const struct stat *statptr, int fileflag, struct FTW *a) {
    if(fileflag==FTW_F)
        files++;
    //printf("%d %d\n", a->base, a->level);
    return 0;
}

int fnDires(const char *filepath, const struct stat *statptr, int fileflag, struct FTW *a) {
    if(fileflag==FTW_D)    
        dires++;
    return 0;
}

int main(){
    FILE * plik=fopen ("plik.txt", "w+");
    int a=fwrite ("Ala ma kota\n", 1, 12, plik);
    fpos_t pos;
    fgetpos (plik, &pos);
    printf("%d\n", pos);
    fseek (plik, 0, 0);
    char dane[11];
    fread (&dane, 1, 11, plik);
    printf("%s\n", dane);
    fclose(plik);

    DIR* dir=opendir(".");
    struct dirent* dirent=readdir(dir);
    while(dirent){
        printf("%d %s\n",dirent->d_ino, dirent->d_name);
        dirent=readdir(dir);
    }

    rewinddir(dir);
    dirent=readdir(dir);
    struct stat fileinfo;
    stat (dirent->d_name, &fileinfo);
    char date[36];
    printf("Access: %s\n", formatdate(date, fileinfo.st_atime));
    printf("Modify: %s\n", formatdate(date, fileinfo.st_mtime));
    printf("Change: %s\n", formatdate(date, fileinfo.st_ctime));
    //printf("%s", asctime(localtime(&fileinfo.st_mtime)));
    closedir(dir);

    chdir ("/home/students/s/z/szielins");
    nftw(".", fnFiles, 100, 0);
    printf("Files number: %d\n", files);
    nftw(".", fnDires, 100, FTW_PHYS);
    printf("Dires number: %d\n", dires);
    return 0;
}