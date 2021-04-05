/*
    Creates N passengers and have them go through a baggage handler,
    security screening, and seating.
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int N = 0;
pthread_mutex_t lock;
//Semaphores for Passengers
sem_t bagSema, secSema, fliSema, pasSema;
//Semaphores for Workers
sem_t bagAvail, secAvail, fliAvail, pasAvail;
int bagPas = 1, secPas = 1, fliPas = 1, pasPas = 1;

// Method that the baggage handlers execute
// Waits for an available baggage handler to be available.
// Processes the passenger and signals security screening
void* B(void* counter) {
    while(bagPas <= N) {
        sem_wait(&bagAvail);
        if(bagPas <= N) {
            pthread_mutex_lock(&lock);

            cout << "Passenger #" << bagPas << " is waiting at baggage processing for a handler\n\n";
            bagPas++;

            pthread_mutex_unlock(&lock);
            usleep(100);
            sem_post(&secSema);
            sem_post(&bagAvail);
        }
    }
    
    return NULL;
}

// Method that the security screeners execute
// Waits for the signal from baggage handlers and for a screener to be available.
// Processes the passenger and signals the flight attendants
void* S(void* counter) {
    while(secPas <= N) {
        sem_wait(&secAvail);
        sem_wait(&secSema);
        if(secPas <= N) {
            pthread_mutex_lock(&lock);

            cout << "Passenger #" << secPas << " is waiting to be screened by a screener\n\n";
            
            secPas++;
            pthread_mutex_unlock(&lock);
            usleep(100);
            sem_post(&fliSema);
            sem_post(&secAvail);
        }
    }

    return NULL;
}

// Method that the flight attendants execute
// Waits for the signal from security screener and for a flight attendant to be available.
// Processes the passenger and signals the passengers.
void* F(void* counter) {
    while(fliPas <= N) {
        sem_wait(&fliSema);
        sem_wait(&fliAvail);
        if(fliPas <= N) {
            pthread_mutex_lock(&lock);

            cout << "Passenger #" << fliPas << " is waiting to be seated by an attendant.\n\n";

            fliPas++;
            pthread_mutex_unlock(&lock);
            usleep(100);
            sem_post(&pasSema);
            sem_post(&fliAvail);
        }
    }
    return NULL;
}
// Method that the passenger execute
// Waits for the signal from flight attendants and for a screener to be available.
// Processes the passenger and signals the flight attendants
void* P(void* counter) {
    while(pasPas <= N) {
        sem_wait(&pasSema);
        sem_wait(&pasAvail);
        if(pasPas <= N) {
            pthread_mutex_lock(&lock);
            
            cout << "Passenger #" << pasPas << " has been seated and relaxes\n\n";

            pasPas++;
            pthread_mutex_unlock(&lock);
            usleep(100);
            sem_post(&pasAvail);
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if(argc < 5) {
        cout << "ERROR: NOT ENOUGH ARGUMENTS";
        return 0;
    }

    // Gets the number of employees of each task and also number of passengers
    N = atoi(argv[1]);
    int numBaggage = atoi(argv[2]);
    int numSecurity = atoi(argv[3]);
    int numFlight = atoi(argv[4]);

    // Creates IDs for each employee for the different tasks
    pthread_t thread_id_baggage[numBaggage];
    pthread_t thread_id_security[numSecurity];
    pthread_t thread_id_flight[numFlight];
    pthread_t thread_id_passenger[N];

    // bagSema with initial value 1 because initially has a passenger available.
    sem_init(&bagSema, 0, 1);
    sem_init(&secSema, 0, 0);
    sem_init(&fliSema, 0, 0);
    sem_init(&pasSema, 0, 0);

    // All with initial value 1 because all employees are initially available.
    sem_init(&bagAvail, 0, 1);
    sem_init(&secAvail, 0, 1);
    sem_init(&fliAvail, 0, 1);
    sem_init(&pasAvail, 0, 1);

    // Creates thread for the number of employees of each task and executes their respective methods.
    for(int i = 0; i < numBaggage; i++) {
        pthread_create(&thread_id_baggage[i], NULL, B, NULL);
    }
    for(int i = 0; i < numSecurity; i++) {
        pthread_create(&thread_id_security[i], NULL, S, NULL);
    }
    for(int i = 0; i < numFlight; i++) {
        pthread_create(&thread_id_flight[i], NULL, F, NULL);
    }
    for(int i = 0; i < N; i++) {
        pthread_create(&thread_id_passenger[i], NULL, P, NULL);
    }

    // Joins the threads of each task
    for(int i = 0; i < numBaggage; i++) {
        pthread_join(thread_id_baggage[i], NULL);
    }
    for(int i = 0; i < numSecurity; i++) {
        pthread_join(thread_id_security[i], NULL);
    }
    for(int i = 0; i < numFlight; i++) {
        pthread_join(thread_id_flight[i], NULL);
    }
    for(int i = 0; i < N; i++) {
        pthread_join(thread_id_passenger[i], NULL);
    }

    sem_destroy(&bagSema);
    sem_destroy(&secSema);
    sem_destroy(&fliSema);
    sem_destroy(&pasSema);
    
    sem_destroy(&bagAvail);
    sem_destroy(&secAvail);
    sem_destroy(&fliAvail);
    sem_destroy(&pasAvail);
    pthread_mutex_destroy(&lock);

    return 0;
}