#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <cstdlib>
#include <iostream>


using namespace std;

#define MAX_SIZE 5
pthread_mutex_t bufLock;
sem_t apple;
sem_t full;
int in = 0;
int out = 0;

char getChar()
{

    char randChar[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                       'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                       's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    return randChar[rand() % 26];
}
//producer gets characters and puts them into buffer
void producer(char *buf)
{

    for (int x = 0; x < 5; x++)
    {
        sem_wait(&apple);
        //lock
        pthread_mutex_lock(&bufLock);
        buf[in] = getChar();
        cout << "producer put: " << buf[in - 1] << " inside buffer[" << in  << "]" << endl;
        in = (in + 1) % MAX_SIZE;
        //unlock
        pthread_mutex_unlock(&bufLock);

        sem_post(&full);
    }
}
//Consumer prints out items in buffer
void consumer(char *buf)
{

    for (int x = 0; x < 5; x++)
    {
        sem_wait(&full);
        //lock
        pthread_mutex_lock(&bufLock);
        //critical
        cout << "consumer took out: " << buf[out] << " from buffer[" << out << "]" << endl;
        //unlock
        pthread_mutex_unlock(&bufLock);
        out = (out + 1) % MAX_SIZE;
        sem_post(&apple);
    }
}

//Tester Func
void Test(int c, int p)
{
    char buffer[MAX_SIZE];
    int threadCount = 2;
    pthread_t thread_id[threadCount];
    //consumer will always read starting from index 0
    out = 0;
    //toProduce % MAX_SIZE
    in = p % MAX_SIZE;
    if (p > 0)
    {
        for (int i = 0; i < p; i++)
        {
            srand(time(NULL));
            buffer[i] = getChar();
        }
    }



    //consume
    sem_init(&apple, 0, c);
    //produce
    sem_init(&full, 0, p);

    pthread_mutex_init(&bufLock, NULL);

    //create threads that will act as producer
    for (int i = 0; i < 1; i++)
    {
        pthread_create(&thread_id[i], NULL, (void *(*)(void *))producer, &buffer);
    }

    //create threads that will act as consumer
    for (int i = 1; i < 2; i++)
    {
        pthread_create(&thread_id[i], NULL, (void *(*)(void *))consumer, &buffer);
    }

    for (int i = 0; i < threadCount; i++)
    {
        pthread_join(thread_id[i], NULL);
    }


    cout << "--------------------------------------" << endl;
}

int main()
{
    //buffer full
    cout<<"Buffer is Full and producer is waiting\n\n";
    Test(5, 0);
    //buffer is empty
    cout<<"Buffer is empty and consumer is waiting\n\n";
    Test(0, 5);
    //buffer is not empty
    cout<<"Buffer is not empty and both producer and consumer are working\n\n";
    Test(1, 1);

    sem_destroy(&apple);
    sem_destroy(&full);
    pthread_mutex_destroy(&bufLock);
    return 0;
}