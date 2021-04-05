//All necessary libraries are imported
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <sys/wait.h>
#include <string>
#include <unistd.h>
#include <list>
#include <iterator>
#include <algorithm>
#include <vector>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <chrono>

using namespace std;
//Create the N int which is the amount of times it will loop
//Create semaphores
int N = 0;
sem_t go, stop;
sem_t ingo, instop;
//Father will read a book and tuck the children to bed
void * father(void* counter){
    for(int x = 0; x<N; x++){
    sem_wait(&stop);
    for(int s = 0; s<12; s++){
        sem_wait(&instop);
        cout<<"Child #"<<s<<" is being read a book \n";
        sem_post(&ingo);
    }
    for(int s = 0; s<12; s++){
        cout<<"Child #"<<s<<" is tucked in bed \n";
    }
    sem_post(&go);
    }
    return NULL;
}
//Mother will wake up, feed breakfast, send to school, give dinner, and give a bath.
//The mother will start off. Once the mother is done with her tasks, the father will wake up and do his. 
void * mother(void* counter){
    for(int d = 0; d<N ; d++){
        sem_wait(&go);
        cout<<"This is day #"<<d<<" of a day in the life of Mother Hubbard.\n";
        if(d > 0){
            cout<<"Father is going to sleep and waking up Mother to take care of the children\n";
        }
        for(int s = 0; s<12; s++){
            cout<<"Child #"<<s<<" is woken up \n";
            usleep(100);
        }
        for(int s = 0; s<12; s++){
            cout<<"Child #"<<s<<" is being fed breakfast \n";
            usleep(100);
        }
        for(int s = 0; s<12; s++){
            cout<<"Child #"<<s<<" is sent to school \n";
            usleep(100);
        }
        for(int s = 0; s<12; s++){
            cout<<"Child #"<<s<<" is given dinner \n";
            usleep(100);
        }
            sem_post(&stop);
        for(int s = 0; s<12; s++){
            sem_wait(&ingo);
            cout<<"Child #"<<s<<" is given a bath \n";
            usleep(100);
            sem_post(&instop);
        }
        cout<<"Mother is going to sleep and waking up Father to take care of the children\n";
    }

    return NULL;
}
//The main will create the mother and father thread
//The main will also initialize the semaphores
int main(int argc, char *argv[]){
    pthread_t thread_id_one;
    pthread_t thread_id_two;
    if(argc > 1) {
        N = atoi(argv[1]);
    }
    else {
        cout << "ERROR: No input\n";
        return 0;
    }

    sem_init(&go, 0, 1);
    sem_init(&stop, 0, 0);
    sem_init(&ingo, 0, 1);
    sem_init(&instop, 0, 0);
    
    pthread_create(&thread_id_one, NULL, mother, NULL);
    pthread_create(&thread_id_two, NULL, father, NULL);

    pthread_join(thread_id_one, NULL);
    pthread_join(thread_id_two, NULL);

    return 0;
}