#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

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

int range;
int threadsAmmount;
Image *new;
pthread_t *threads;

void readImage(File* imageToRead) {
    char *line = malloc(10);
    size_t size = 2;
    getline(&line, &size, imageToRead);
    if(strcmp(line, "P2")) {
		printf("Wrong file format");
		exit(0);
	}
    size = 9;
    getline(&line, &size, imageToRead);
    image = malloc(sizeof(Image));
    sscanf(line, "%d %d\n", &(image->width), &(image->height));
	size=3;
    getline(&line, &size, imageToRead);
    if(strcmp(line, "255")) {
		printf("Wrong grayscale");
		exit(0);
	}
	free(line);
    image->data = calloc(image->height, sizeof(unsigned char*));
    int i;
	char *value=malloc(3);
    for(i = 0; i < image->height; i++) {
        image->data[i] = calloc(image->width, sizeof(unsigned char));
        int j;
        for(j = 0; j < image->width; j++) {
            fscanf(imageToRead, "%s", &value);
            image->data[i][j] = atoi(value);
        }
    }
	free(value);
}  

void readFilter(File* filterToRead) {
    char *line = malloc(20);
    size_t size = 20;
    getline(&line, &size, filterToRead);
    filter = malloc(sizeof(Filter));
    filter->size = atoi(line);
    filter->data = malloc(filter->size*sizeof(float*));
    int i, j;
    char *val = malloc(10);
    for(i = 0; i < filter->size; i++) {
        filter->data[i] = malloc(filter->size*sizeof(float));
        size_t size = 10;
        for(j = 0; j < filter->size; j++) {
            getdelim(&val, &size, ' ', filterToRead);
            filter->data[i][j] = strtof(val, NULL);
        }
    }
}

Image *create_empty(int width, int height) {
    Image *image = malloc(sizeof(Image));
    image->data = calloc(height*sizeof(char*), 1);
    image->width = width;
    image->height = height;
    int i;
    for(i = 0; i < height; i++) {
        image->data[i] = calloc(width, 1);
    }
    return image;
}

void save_image(Image *image, char *name) {
    FILE *fd = fopen(name, "w+");
    if(fd == NULL) error_exit("File opening failure");
    char *line = malloc(20);
    fwrite("P2\n", 1, 3, fd);
    int len = sprintf(line, "%d %d\n", image->width, image->height);
    fwrite(line, 1, len, fd);
    fwrite("255\n", 1, 4, fd);
    int counter = 0;
    int i, j;
    for(i = 0; i < image->height; i++) {
        for(j = 0; j < image->width; j++) {
            counter++;
            if(counter < 17) {
                fprintf(fd, "%3u ", image->data[i][j]);
            } else {
                counter = 0;
                fprintf(fd, "%3u \n", image->data[i][j]);
            }
        }
    }
    fclose(fd);
}

void filter(Image *new, Image *image, Filter *filter, int x, int y) {
    double sum = 0;
    int i, j;
    for(i = 0; i < filter->size; i++) {
        for(j = 0; j < filter->size; j++) {
            sum += image->data[(int) (fmax(1, fmin(image->height - 1, x - ceil(filter->size/2) + i + 1)))]
                [(int) (fmax(1, fmin(image->width -1, y - ceil(filter->size/2) + i + 1)))]*filter->data[i][j];
        }
    }
    new->data[x][y] = (unsigned char) round(sum);
}

void *filter_block(void *ptr) {
    int k = *((int*) ptr);
    int i, j;
    struct timeval start, end;
    gettimeofday(&start,NULL);
    for(j = k*ceil(range); j < (k+1)*ceil(range); j++) {
        for(i = 0; i < image->height; i++) {
            filter(new, image, fil, i, j);
        }
    }
    gettimeofday(&end,NULL);
    int *elapsed = malloc(sizeof(int));
    *elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    pthread_exit((void*) elapsed);
}

void *filter_intervaled(void *ptr) {
    int k = *((int*) ptr);
    int i, j;
    struct timeval start, end;
    gettimeofday(&start,NULL);
    for(j = k; j < image->width; j += threadsAmmount) {
        for(i = 0; i < image->height; i++) {
            filter(new, image, fil, i, j);
        }
    }
    gettimeofday(&end,NULL);
    int *elapsed = malloc(sizeof(int));
    *elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    pthread_exit((void*) elapsed);
}

void filter_parallel(char *mode) {
    range = image->width/threadsAmmount;
    threads = malloc(threadsAmmount*sizeof(pthread_t));
    int i;
    struct timeval start, end;
    int times[threadsAmmount];
    gettimeofday(&start, NULL);
    for(i = 0; i < threadsAmmount; i++) {
        int *a = malloc(sizeof(int));
        memcpy(a, &i, sizeof(int)); 
        if(strcmp(mode, "block") == 0) {
            pthread_create(&(threads[i]), NULL, filter_block, a);
        } else if(strcmp(mode, "intervaled") == 0) {
            pthread_create(&(threads[i]), NULL, filter_intervaled, a);
        } else {
            fprintf(stderr, "Incorrect mode\n");
            exit(-1);
        }
    }
    for(i = 0; i < threadsAmmount; i++) {
        int *retval;
        if(pthread_join(threads[i], (void**) &retval) != 0) error_exit("pthread_join");
        times[i] = *retval;
        free(retval);
    }
    gettimeofday(&end, NULL);
    for(i = 0; i < threadsAmmount; i++) {
        printf("Thread %d, time: %d us\n", i, times[i]);
    }
    int elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    printf("Total time: %d us\n", elapsed);
}

bool parser(char** argv, int argc) {
	if(argc != 6) {
		printf("Wrong number of arguments\n");
		return false;
	}
	if (!atoi(argv[1])) {
		printf ("argument 1 musi byc liczba\n");
		return false;
	}
	if (!strcmp(argv[2], "block") || !strcmp(argv[2], "interleaved")) {
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
}

int main(int argc, char **argv) {
	if(parser(argv, argc)){
		FILE *resultImage = fopen(argv[5], "w+");
		if (resultImage==NULL) {
			printf ("nie mozna otworzyc pliku podanego jako 5 argument\n");
			help();
			return -1;
		}
		int threadsAmmount = atoi(argv[1]);
		char *mode = argv[2];
		new = create_empty(image->width, image->height);
		filter_parallel(mode);
		save_image(new, output);
	}
	else help();
    return 0;
}