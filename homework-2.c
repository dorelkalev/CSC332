#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Constants for the maximum number and thread count
const int MAX_NUM = 10000;
const int THREAD_COUNT = 4;

//Structure to hold data for each thread
struct ThreadData {
    int start;
    int end;
    int max_divisors;
    int number_with_max_divisors;
};

//Function to find the number with the maximum divisors in a given range
void *find_max_divisors(void *arg) {
struct ThreadData *data = (struct ThreadData *)arg;
    int max_divisors = 0;
    int number_with_max = data->start;

    for (int i = data->start; i <= data->end; i++) {
        int count = 0;
        for (int j = 1; j <= i; j++) {
            if (i % j == 0) {
                count++;
            }
        }
        if (count > max_divisors) {
            max_divisors = count;
            number_with_max = i;
        }
    }

    //Store the results in the thread data structure
    data->max_divisors = max_divisors;
    data->number_with_max_divisors = number_with_max;
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];
    struct ThreadData thread_data[THREAD_COUNT];
    int range = MAX_NUM / THREAD_COUNT;
    clock_t start_time = clock();

  
    //Create threads to find max divisors in their assigned ranges
    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_data[i].start = i * range + 1;
        thread_data[i].end = (i + 1) * range;
        thread_data[i].max_divisors = 0;
        thread_data[i].number_with_max_divisors = 0;
        pthread_create(&threads[i], NULL, find_max_divisors, (void *)&thread_data[i]);
    }

    //Wait for all threads to complete
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    int global_max_divisors = 0;
    int global_number_with_max_divisors = 0;

    //Determine the overall maximum divisors and corresponding number
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (thread_data[i].max_divisors > global_max_divisors) {
            global_max_divisors = thread_data[i].max_divisors;
            global_number_with_max_divisors = thread_data[i].number_with_max_divisors;
        }
    }

    //End timing the operation
    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Number with the most divisors: %d\n", global_number_with_max_divisors);
    printf("Time spent: %f seconds\n", time_spent);

    return 0;
}
