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
    int Trolley;
} Passenger;

typedef struct Trolley{
    int id;
    int size;
    int tripsAmmount;
    Passenger* Passengers;
} Trolley;

int trolleysCount;
int trolleySize;
pthread_cond_t* Trolleys_cond;
pthread_mutex_t* Trolleys_mutex;

pthread_mutex_t stationBusy_mutex;
pthread_mutex_t passenger_mutex;
pthread_mutex_t emptyTrolley_mutex;
pthread_mutex_t fullTrolley_mutex;

pthread_cond_t empty_condition;
pthread_cond_t full_condition;

Trolley* Trolleys;
int station_Trolley_id;
struct timeval get_current_time() {
    struct timeval curr;
    gettimeofday(&curr, NULL);
    return curr;
}

void *run_Passenger(void *data) {
    Passenger *Passenger = (Passenger *) data;
    while (1) {
        pthread_mutex_lock(&passenger_mutex);

        Passenger->Trolley = station_Trolley_id;
        Trolleys[station_Trolley_id].Passengers[Trolleys[station_Trolley_id].size] = *Passenger;
        Trolleys[station_Trolley_id].size = Trolleys[station_Trolley_id].size + 1;
        struct timeval curr = get_current_time();
        printf("Passenger %d entered the Trolley, people inside: %d, time: %ld.%06ld \n", Passenger->id,Trolleys[station_Trolley_id].size, curr.tv_sec, curr.tv_usec);

        if(Trolleys[station_Trolley_id].size == trolleySize){
            srand(time(NULL));
            curr = get_current_time();
            printf("Passenger %d pressed start, people inside: %d, time: %ld.%06ld\n", Trolleys[station_Trolley_id].Passengers[rand()%Trolleys->size].id,Trolleys[station_Trolley_id].size, curr.tv_sec, curr.tv_usec);
            pthread_cond_signal(&full_condition);
            pthread_mutex_unlock(&fullTrolley_mutex);
        } else {
            pthread_mutex_unlock(&passenger_mutex);
        }

        pthread_mutex_lock(&Trolleys_mutex[Passenger->Trolley]);
        curr = get_current_time();
        Trolleys[station_Trolley_id].size--;
        printf("Passenger %d left, people in Trolley: %d, time: %ld.%06ld \n", Passenger->id, Trolleys[station_Trolley_id].size,curr.tv_sec, curr.tv_usec);
        if(Trolleys[station_Trolley_id].size == 0){
            pthread_cond_signal(&empty_condition);
            pthread_mutex_unlock(&emptyTrolley_mutex);
        }
        pthread_mutex_unlock(&Trolleys_mutex[Passenger->Trolley]);
        Passenger->Trolley = -1;
    }
}

void *run_Trolley(void *data) {
    Trolley *Trolley = (Trolley *) data;
    if (Trolley->id == 0)
        pthread_mutex_lock(&passenger_mutex);
    int i;
    for (i = 0; i < Trolley->tripsAmmount; i++) {
        pthread_mutex_lock(&stationBusy_mutex);
        if (Trolley->id != station_Trolley_id) {
            pthread_cond_wait(&Trolleys_cond[Trolley->id], &stationBusy_mutex);
        }
        struct timeval curr = get_current_time();
        printf("Trolley %d arrived, time: %ld.%06ld\n", Trolley->id, curr.tv_sec, curr.tv_usec);

        if (i != 0) {
            pthread_mutex_unlock(&Trolleys_mutex[Trolley->id]);
            pthread_cond_wait(&empty_condition, &emptyTrolley_mutex);
        }

        pthread_mutex_lock(&Trolleys_mutex[Trolley->id]);
        pthread_mutex_unlock(&passenger_mutex);
        pthread_cond_wait(&full_condition, &fullTrolley_mutex);

        curr = get_current_time();
        printf("Trolley %d is full, time: %ld.%06ld\n", Trolley->id, curr.tv_sec, curr.tv_usec);
        station_Trolley_id = (station_Trolley_id + 1) % trolleysCount;

        pthread_cond_signal(&Trolleys_cond[station_Trolley_id]);
        pthread_mutex_unlock(&stationBusy_mutex);
    }

    pthread_mutex_lock(&stationBusy_mutex);

    if(Trolley->id != station_Trolley_id) {
        pthread_cond_wait(&Trolleys_cond[Trolley->id], &stationBusy_mutex);
    }

    struct timeval curr = get_current_time();
    printf("Trolley %d arrived, time: %ld.%06ld\n", Trolley->id, curr.tv_sec, curr.tv_usec);

    station_Trolley_id = Trolley->id;

    pthread_mutex_unlock(&Trolleys_mutex[Trolley->id]);
    pthread_cond_wait(&empty_condition,&emptyTrolley_mutex);

    station_Trolley_id = (station_Trolley_id + 1)%trolleysCount;

    curr = get_current_time();
    printf("Trolley %d finished, time: %ld.%06ld\n", Trolley->id, curr.tv_sec, curr.tv_usec);

    pthread_cond_signal(&Trolleys_cond[station_Trolley_id]);
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
    int trolleysCount = atoi(argv[2]);
    int trolleySize = atoi(argv[3]);
    int raidCount = atoi(argv[4]);

    station_Trolley_id = 0;
    pthread_t passengersThreads[passengersCount];
    pthread_t trolleysThreads[trolleysCount];
    Passenger Passengers[passengersCount];
    Trolleys = malloc(sizeof(Trolley) * trolleysCount);
    for(i=0; i<trolleysCount; i++){
         Trolley->Passengers=malloc(sizeof(int) * trolleySize);
    }
    Trolleys_mutex = malloc(sizeof(pthread_mutex_t) * trolleysCount);
    Trolleys_cond = malloc(sizeof(pthread_cond_t) * trolleysCount);

    pthread_mutex_init(&stationBusy_mutex, NULL);
    pthread_mutex_init(&emptyTrolley_mutex, NULL);
    pthread_mutex_init(&passenger_mutex, NULL);
    pthread_mutex_init(&fullTrolley_mutex, NULL);
    pthread_cond_init(&empty_condition, NULL);
    pthread_cond_init(&full_condition, NULL);

    int i;
    for (i = 0; i < passengersCount; i++) {
        Passengers[i].id = i;
        Passengers[i].Trolley = -1;
    }

    for (i = 0; i < trolleysCount; i++) {
        Trolleys[i].id = i;
        Trolleys[i].size = trolleySize;
        Trolleys[i].tripsAmmount = raidCount;
        Trolleys[i].Passengers = malloc(sizeof(Passenger) * trolleySize);
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
        free(Trolleys[i].Passengers);
    }
    free(Trolleys);
    free(Trolleys_mutex);
    free(Trolleys_cond);
    pthread_exit(NULL);
}