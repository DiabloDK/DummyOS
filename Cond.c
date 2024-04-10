#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Added for sleep()

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0; // Number of items in the buffer
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    int item;
    while (1) {
        item = rand() % 100; // Generate a random item

        pthread_mutex_lock(&mutex);
        if (count == BUFFER_SIZE) {
            pthread_cond_wait(&full, &mutex); // Buffer is full, wait for consumer
        }
        buffer[count++] = item; // Produce item and add to buffer
        printf("Produced: %d\n", item);
        pthread_cond_signal(&empty); // Signal to consumer that buffer is not empty
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 2); // Simulate variable production time
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (count == 0) {
            pthread_cond_wait(&empty, &mutex); // Buffer is empty, wait for producer
        }
        item = buffer[--count]; // Consume item from buffer
        printf("Consumed: %d\n", item);
        pthread_cond_signal(&full); // Signal to producer that buffer is not full
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 2); // Simulate variable consumption time
    }
    return NULL;
}

int main() {
    pthread_t prod_tid, cons_tid;

    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid, NULL, consumer, NULL);

    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    return 0;
}
