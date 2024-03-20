#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 2
#define NUM_WRITERS 2

pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t write_cond = PTHREAD_COND_INITIALIZER;
int readers_count = 0;
int is_writing = 0;

void *reader(void *arg) {
    while (1) {
        pthread_mutex_lock(&file_mutex);
        while (is_writing) {
            pthread_cond_wait(&write_cond, &file_mutex);
        }
        readers_count++;
        printf("Reader %ld is reading.\n", (long)arg);
        if (readers_count == 1) {
            printf("There is no reader.\n");
        }
        pthread_mutex_unlock(&file_mutex);
        usleep(1000000); // Sleep for 1 second
        pthread_mutex_lock(&file_mutex);
        readers_count--;
        if (readers_count == 0) {
            pthread_cond_signal(&write_cond); // Signal writer if no readers left
        }
        pthread_mutex_unlock(&file_mutex);
        usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

void *writer(void *arg) {
    while (1) {
        pthread_mutex_lock(&file_mutex);
        while (readers_count > 0 || is_writing) {
            pthread_cond_wait(&write_cond, &file_mutex);
        }
        is_writing = 1;
        printf("Writer %ld is writing.\n", (long)arg);
        pthread_mutex_unlock(&file_mutex);
        usleep(2000000); // Sleep for 2 seconds
        pthread_mutex_lock(&file_mutex);
        is_writing = 0;
        printf("Writer %ld exited.\n", (long)arg);
        pthread_cond_broadcast(&write_cond); // Signal waiting readers
        pthread_mutex_unlock(&file_mutex);
        usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    long i;

    // Create reader threads
    for (i = 0; i < NUM_READERS; i++) {
        pthread_create(&readers[i], NULL, reader, (void *)i);
    }

    // Create writer threads
    for (i = 0; i < NUM_WRITERS; i++) {
        pthread_create(&writers[i], NULL, writer, (void *)i);
    }

    // Join reader threads
    for (i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    // Join writer threads
    for (i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}
