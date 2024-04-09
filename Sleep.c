#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h> // Added for sleep function
#include <time.h>   // Added for time function

sem_t empty, full;
pthread_mutex_t mutex;
int count = 0;
int buffer[10];

void* producerstud(void* args) {
    int num = (int)args;
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    buffer[count] = rand() % 100;
    printf("Producer %d Question %d\n", num + 1, buffer[count]);
    count++;
    sleep(1);
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return NULL;
}

void* consumerprof(void* args) {
    int num = (int)args;
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    count--;
    printf("Consumer %d ans %d\n", num + 1, buffer[count]);
    if (count == 0)
        printf("Professor is sleeping\n");
    sleep(1);
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return NULL;
}

int main() {
    srand(time(NULL));

    int numberOfProducers, numberOfConsumers;
    printf("Enter the number of producersstud: ");
    scanf("%d", &numberOfProducers);
    printf("Enter the number of consumersprof: ");
    scanf("%d", &numberOfConsumers);

    sem_init(&empty, 0, 10); // Initialize empty semaphore with buffer size
    sem_init(&full, 0, 0);   // Initialize full semaphore with 0

    pthread_mutex_init(&mutex, NULL);

    pthread_t prod[numberOfProducers], cons[numberOfConsumers]; // Adjusted array sizes

    unsigned long int i;
    for (i = 0; i < numberOfProducers; i++) {
        pthread_create(&prod[i], NULL, &producerstud, (void *)i);
    }
    for (i = 0; i < numberOfConsumers; i++) { // Changed to numberOfConsumers
        pthread_create(&cons[i], NULL, &consumerprof, (void *)i);
    }
    for (i = 0; i < numberOfProducers; i++) {
        pthread_join(prod[i], NULL);
    }
    for (i = 0; i < numberOfConsumers; i++) { // Changed to numberOfConsumers
        pthread_join(cons[i], NULL);
    }

    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);

    return 0;
}
