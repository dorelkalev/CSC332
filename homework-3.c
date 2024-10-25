#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_FARMERS 5

pthread_mutex_t bridge_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bridge_cond = PTHREAD_COND_INITIALIZER;
int current_direction = 0; //0: none, 1: north, 2: south
int north_count = 0;
int south_count = 0;

void *farmer(void *arg) {
    int id = *((int *)arg);
    int direction = id % 2; //Even IDs go North, Odd IDs go South
    const char *dir_str = direction == 0 ? "North" : "South";

    pthread_mutex_lock(&bridge_mutex);
    //Wait until the bridge is free for the direction
    while ((current_direction != 0 && current_direction != direction) ||
           (direction == 0 && south_count > 0) || (direction == 1 && north_count > 0)) {
        if (direction == 0) north_count++;
        else south_count++;
        pthread_cond_wait(&bridge_cond, &bridge_mutex);
        if (direction == 0) north_count--;
        else south_count--;
    }
    // Set the direction
    current_direction = direction;

    printf("%s Tunbridge farmer #%d can cross the bridge\n", dir_str, id);
    pthread_mutex_unlock(&bridge_mutex);

    // Simulate crossing the bridge
    printf("%s Tunbridge farmer #%d is traveling on the bridge...\n", dir_str, id);
    sleep(rand() % 4); //Sleep for 0 to 3 seconds

    pthread_mutex_lock(&bridge_mutex);
    printf("%s Tunbridge farmer #%d has left the bridge\n", dir_str, id);
    current_direction = 0; //Bridge is now free
    pthread_cond_broadcast(&bridge_cond); //Wake up waiting farmers
    pthread_mutex_unlock(&bridge_mutex);

    return NULL;
}

int main() {
    pthread_t farmers[NUM_FARMERS];
    int farmer_ids[NUM_FARMERS];

    srand(time(NULL));

    for (int i = 0; i < NUM_FARMERS; i++) {
        farmer_ids[i] = i + 1; //Farmer IDs start from 1
        pthread_create(&farmers[i], NULL, farmer, &farmer_ids[i]);
    }

    for (int i = 0; i < NUM_FARMERS; i++) {
        pthread_join(farmers[i], NULL);
    }

    return 0;
}
