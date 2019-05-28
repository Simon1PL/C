#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

typedef struct Passenger{
    int id;
    int trolley;
} Passenger;

typedef struct Trolley{
    int id;
    int busyPlaces;
    int tripsAmmount;
    int size;
    Passenger* passengers;
} Trolley;

struct timeval currentTime;
int trolleysCount;

pthread_cond_t* Trolleys_cond;
pthread_mutex_t* Trolleys_mutex;

pthread_mutex_t stationBusy_mutex;
pthread_mutex_t passenger_mutex;
pthread_mutex_t emptyTrolley_mutex;
pthread_mutex_t fullTrolley_mutex;

pthread_cond_t empty_condition;
pthread_cond_t full_condition;

Trolley* Trolleys;
int trolleyOnStationID;

void *run_Passenger(void *data) {
    Passenger *passenger = (Passenger *) data;
    while (1) {
        pthread_mutex_lock(&passenger_mutex);
        passenger->trolley = trolleyOnStationID;
        Trolleys[trolleyOnStationID].passengers[Trolleys[trolleyOnStationID].busyPlaces] = *Passenger;
        Trolleys[trolleyOnStationID].busyPlaces += 1;
        gettimeofday(&currentTime, NULL);
        printf("Passenger number %d entered the Trolley, people inside: %d, time: %ld.%06ld\n", Passenger->id,Trolleys[trolleyOnStationID].busyPlaces, time.tv_sec, time.tv_usec);

        if(Trolleys[trolleyOnStationID].busyPlaces == Trolleys[trolleyOnStationID].size){
            srand(time(NULL));
            gettimeofday(&currentTime, NULL);
            printf("Passenger number %d pressed start, time: %ld.%06ld\n", Trolleys[trolleyOnStationID].passengers[rand()%Trolleys[trolleyOnStationID].size].id, time.tv_sec, time.tv_usec);
            pthread_cond_signal(&full_condition);
            pthread_mutex_unlock(&fullTrolley_mutex);
        } else {
            pthread_mutex_unlock(&passenger_mutex);
        }

        pthread_mutex_lock(&Trolleys_mutex[Passenger->trolley]);
        gettimeofday(&currentTime, NULL);
        Trolleys[trolleyOnStationID].busyPlaces++;
        printf("Passenger number %d left, people in Trolley: %d, time: %ld.%06ld \n", Passenger->id, Trolleys[trolleyOnStationID].busyPlaces,time.tv_sec, time.tv_usec);
        if(Trolleys[trolleyOnStationID].busyPlaces == Trolleys[trolleyOnStationID].size){
            pthread_cond_signal(&empty_condition);
            pthread_mutex_unlock(&emptyTrolley_mutex);
        }
        pthread_mutex_unlock(&Trolleys_mutex[Passenger->Trolley]);
        Passenger->trolley = -1;
    }
}

void *run_Trolley(void *data) {
    Trolley *trolley = (Trolley *) data;
    if (trolley->id == 0)
        pthread_mutex_lock(&passenger_mutex);
    int i;
    for (i = 0; i < trolley->tripsAmmount; i++) {
        pthread_mutex_lock(&stationBusy_mutex);
        if (trolley->id != trolleyOnStationID) {
            pthread_cond_wait(&Trolleys_cond[trolley->id], &stationBusy_mutex);
        }
        gettimeofday(&currentTime, NULL);
        printf("Trolley %d arrived, time: %ld.%06ld\n", trolley->id, time.tv_sec, time.tv_usec);

        if (i != 0) {
            pthread_mutex_unlock(&Trolleys_mutex[trolley->id]);
            pthread_cond_wait(&empty_condition, &emptyTrolley_mutex);
        }

        pthread_mutex_lock(&Trolleys_mutex[trolley->id]);
        pthread_mutex_unlock(&passenger_mutex);
        pthread_cond_wait(&full_condition, &fullTrolley_mutex);

        gettimeofday(&currentTime, NULL);
        printf("Trolley %d is full, time: %ld.%06ld\n", trolley->id, time.tv_sec, time.tv_usec);
        gettimeofday(&currentTime, NULL);
        printf("Trolley %d closed the door, time: %ld.%06ld\n", trolley->id, time.tv_sec, time.tv_usec);
        trolleyOnStationID = (trolleyOnStationID + 1) % trolleysCount;

        pthread_cond_signal(&Trolleys_cond[trolleyOnStationID]);
        pthread_mutex_unlock(&stationBusy_mutex);
    }

    pthread_mutex_lock(&stationBusy_mutex);

    if(trolley->id != trolleyOnStationID) {
        pthread_cond_wait(&Trolleys_cond[trolley->id], &stationBusy_mutex);
    }

    gettimeofday(&currentTime, NULL);
    printf("Trolley %d arrived, time: %ld.%06ld\n", trolley->id, time.tv_sec, time.tv_usec);
    gettimeofday(&currentTime, NULL);
    printf("Trolley %d opend the door, time: %ld.%06ld\n", trolley->id, time.tv_sec, time.tv_usec);

    trolleyOnStationID = trolley->id;

    pthread_mutex_unlock(&Trolleys_mutex[trolley->id]);
    pthread_cond_wait(&empty_condition,&emptyTrolley_mutex);

    trolleyOnStationID = (trolleyOnStationID + 1)%trolleysCount;

    gettimeofday(&currentTime, NULL);
    printf("Trolley %d finished, time: %ld.%06ld\n", trolley->id, time.tv_sec, time.tv_usec);
    
    pthread_cond_signal(&Trolleys_cond[trolleyOnStationID]);
    pthread_mutex_unlock(&stationBusy_mutex);

    pthread_exit(NULL);
}

void help(){
    printf("use: ./main ammountOfPassengers ammountOfTrolleys trolleySize ammountOfTrolleyTrips\neg. ./main 10 3 3 2");
    exit(0);
}

int main(int argc, char **argv) {
    if (argc != 5){
        printf("Wrong number of arguments");
        help();
    }
    int i;
    for(i=1; i<5; i++) {
        if (!atoi(argv[i])) {
            printf("argument %d has to be a number", i);
            help();
        }
         if (atoi(argv[i])<=0) {
            printf("argument %d has to be greater than zero", i);
            help();
        }
        if(atoi(argv[2])*atoi(argv[3])<atoi(argv[1])){
            printf("the ammount of people has to be greater than the number of seats", i);
            help();
        }
    }
    int passengersCount = atoi(argv[1]);
    trolleysCount = atoi(argv[2]);
    int trolleySize = atoi(argv[3]);
    int raidCount = atoi(argv[4]);

    trolleyOnStationID = 0;
    pthread_t passengersThreads[passengersCount];
    pthread_t trolleysThreads[trolleysCount];
    Passenger Passengers[passengersCount];
    Trolleys = malloc(sizeof(Trolley) * trolleysCount);
    for(i=0; i<trolleysCount; i++){
         trolley->Passengers=malloc(sizeof(int) * trolleySize);
    }
    Trolleys_mutex = malloc(sizeof(pthread_mutex_t) * trolleysCount);
    Trolleys_cond = malloc(sizeof(pthread_cond_t) * trolleysCount);

    pthread_mutex_init(&stationBusy_mutex, NULL);
    pthread_mutex_init(&emptyTrolley_mutex, NULL);
    pthread_mutex_init(&passenger_mutex, NULL);
    pthread_mutex_init(&fullTrolley_mutex, NULL);
    pthread_cond_init(&empty_condition, NULL);
    pthread_cond_init(&full_condition, NULL);

    for (i = 0; i < passengersCount; i++) {
        Passengers[i].id = i;
        Passengers[i].trolley = -1;
    }

    for (i = 0; i < trolleysCount; i++) {
        Trolleys[i].id = i;
        Trolleys[i].size = trolleySize;
        Trolleys[i].busyPlaces = 0;
        Trolleys[i].tripsAmmount = raidCount;
        Trolleys[i].passengers = malloc(sizeof(Passenger) * trolleySize);
        pthread_mutex_init(&Trolleys_mutex[i], NULL);
        pthread_cond_init(&Trolleys_cond[i], NULL);
    }

    for (i = 0; i < trolleysCount; i++) {
        pthread_create(&trolleysThreads[i], NULL, run_Trolley, &Trolleys[i]);
    }
    for (i = 0; i < passengersCount; i++) {
        pthread_create(&passengersThreads[i], NULL, run_Passenger, &Passengers[i]);
    }

    for (i = 0; i < trolleysCount; i++) {
        pthread_join(trolleysThreads[i], NULL);
    }
    for (i = 0; i < trolleysCount; i++) {
        pthread_mutex_destroy(&Trolleys_mutex[i]);
    }
    for(i = 0; i < trolleysCount; i++){
        free(Trolleys[i].passengers);
    }
    free(Trolleys);
    free(Trolleys_mutex);
    free(Trolleys_cond);
    pthread_exit(NULL);
}