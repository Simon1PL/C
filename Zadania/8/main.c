#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct{
    int width;
    int height;
    unsigned char **data;
} Image;

typedef struct{
    int size;
    float **data;
} Filter;

Image *image;
Filter *filter;
Image *filteredImage;
int threadsAmmount;

void help(){
    printf("Program przyjmuje w argumentach wywołania:\n");
    printf("1. liczbę wątków\n");
    printf("2. sposób podziału obrazu pomiędzy wątki, t.j. jedną z dwóch opcji: block / interleaved\n");
    printf("3. nazwę pliku z wejściowym obrazem\n");
    printf("4. nazwę pliku z definicją filtru\n");
    printf("5. nazwę pliku wynikowego\n");
}

void readImage(FILE *imageToRead) {
    char *line = malloc(10);
    size_t size = 2;
    getline(&line, &size, imageToRead);
    if(strcmp(line, "P2\n")) {
		printf("Wrong file format\n");
		exit(0);
	}
    size = 9;
    getline(&line, &size, imageToRead);
    image = malloc(sizeof(Image));
    sscanf(line, "%d %d\n", &(image->width), &(image->height));
	size=3;
    getline(&line, &size, imageToRead);
    if(strcmp(line, "255\n")) {
		printf("Wrong grayscale");
		exit(0);
	}
	free(line);
    image->data = calloc(image->width, sizeof(char*));
    int i;
	char *value=malloc(4);
    for(i = 0; i < image->width; i++) {
        image->data[i] = calloc(image->height, sizeof(char));
        int j;
        for(j = 0; j < image->height; j++) {
            fscanf(imageToRead, "%s", value);
            image->data[i][j] = atoi(value);
        }
    }
	free(value);
}  

void readFilter(FILE *filterToRead) {
    char *line = malloc(5);
    size_t size = 5;
    getline(&line, &size, filterToRead);
    filter = malloc(sizeof(Filter));
    filter->size = atoi(line);
    filter->data = calloc(filter->size, sizeof(float*));
    int i, j;
    char *value = malloc(10);
    for(i = 0; i < filter->size; i++) {
        filter->data[i] = calloc(filter->size, sizeof(float));
        size_t size = sizeof(float);
        for(j = 0; j < filter->size; j++) {
            fscanf(filterToRead, "%f", value);
            filter->data[i][j] = strtof(value, NULL);
        }
    }
}

Image *createNewImage(int width, int height) {
    Image *image = malloc(sizeof(Image));
    image->data = calloc(height, sizeof(unsigned char*));
    image->width = width;
    image->height = height;
    int i;
    for(i = 0; i < height; i++) {
        image->data[i] = calloc(width, sizeof(unsigned char));
    }
    return image;
}

void filterMachine(Image *new, Image *old, Filter *filter, int x, int y) {
    double sum = 0;
    int i, j;
    for(i = 0; i < filter->size; i++) {
        for(j = 0; j < filter->size; j++) {
            sum += image->data[(int) (fmax(1, fmin(image->width - 1, x - ceil(filter->size/2) + i + 1)))]
                [(int) (fmax(1, fmin(image->height -1, y - ceil(filter->size/2) + j + 1)))]*filter->data[i][j];
        }
    }
    new->data[x][y] = (unsigned char) round(sum);
}

void *blockFilter(void *threadNumber) {
    int k = *((int*) threadNumber);
    int i, j;
    struct timeval start, end;
    float range=image->width/threadsAmmount;
    gettimeofday(&start,NULL);
    for(i = k*ceil(range); i < (k+1)*ceil(range); i++) {
        for(j = 0; j < image->height; j++) {
            filterMachine(filteredImage, image, filter, i, j);
        }
    }
    gettimeofday(&end,NULL);
    int timeDifference=malloc(sizeof(int));
    timeDifference = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    return ((void*)timeDifference);
}

void *InterleavedFilter(void *threadNumber) {
    int k =  *((int*) threadNumber);
    int i, j;
    struct timeval start, end;
    gettimeofday(&start,NULL);
    for(i = k; i < image->width; i += threadsAmmount) {
        for(j = 0; j < image->height; j++) {
            filterMachine(filteredImage, image, filter, i, j);
        }
    }
    gettimeofday(&end,NULL);
    int timeDifference=malloc(sizeof(int));
    timeDifference = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    return ((void*)timeDifference);
}

void makeFilter(char *mode, int i, pthread_t *threads) {
   if(!strcmp(mode, "block")) {
        pthread_create(&(threads[i]), NULL, blockFilter, &i);
    } 
    else {
        pthread_create(&(threads[i]), NULL, InterleavedFilter, &i);
    } 
}

void save_image(Image *image, FILE *file) {
    fwrite("P2\n", 1, 3, file);
    char *line = malloc(10);
    int len = sprintf(line, "%d %d\n", image->width, image->height);
    fwrite(line, 1, len, file);
    fwrite("255", 1, 4, file);
    int i, j;
    for(i = 0; i < image->width; i++) {
        fprintf(file, "\n");
        for(j = 0; j < image->height; j++) 
            fprintf(file, "%u ", image->data[i][j]);
    }
}

bool parser(char** argv, int argc) {
	if(argc != 6) {
		printf("Wrong number of arguments(%d), should be 6\n", argc);
		return false;
	}
	if (!atoi(argv[1])) {
		printf ("argument 1 musi byc liczba\n");
		return false;
	}
	if (strcmp(argv[2], "block") && strcmp(argv[2], "interleaved")) {
		printf ("argument 2 musi byc  jedną z dwóch opcji: block / interleaved\n");
		return false;
	}
	FILE *startImage = fopen(argv[3], "r");
	if (startImage==NULL) {
		printf ("nie mozna otworzyc pliku podanego jako 3 argument\n");
		return false;
	}
	FILE *filterFile = fopen(argv[4], "r");
	if (filterFile==NULL) {
		printf ("nie mozna otworzyc pliku podanego jako 4 argument\n");
		return false;
	}
	readImage(startImage);
	readFilter(filterFile);
	fclose(startImage);
	fclose(filterFile);
    return true;
}

int main(int argc, char **argv) {
	if(parser(argv, argc)){
		FILE *resultImage = fopen(argv[5], "a");
		if (resultImage==NULL) {
			printf ("nie mozna otworzyc pliku podanego jako 5 argument\n");
			help();
			return -1;
		}
		threadsAmmount = atoi(argv[1]);
		char *mode = argv[2];
		filteredImage = createNewImage(image->width, image->height);
        pthread_t *threads;
        threads = calloc(threadsAmmount, sizeof(pthread_t));
        struct timeval start, end;
        int times[threadsAmmount];
        gettimeofday(&start, NULL);
        int i;
        for(i = 0; i < threadsAmmount; i++) {
            makeFilter(mode, i, threads);
        }
        for(i = 0; i < threadsAmmount; i++) {
            if(pthread_join(threads[i], (void**) &times[i]) != 0) {
                printf("blad przy odczycie returna watku\n");
                exit(0);
            }
        }
        gettimeofday(&end, NULL);
        for(i = 0; i < threadsAmmount; i++) {
            printf("Thread: %d, time: %dus\n", i+1, times[i]);
        }
        int timeDifference = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
        printf("Total time: %dus\n", timeDifference);
        save_image(filteredImage, resultImage);
    }
	else help();
    return 0;
}