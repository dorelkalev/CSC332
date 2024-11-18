#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_CYLINDERS 10000
#define NUM_REQUESTS 1000

int compare(const int* a, const int* b) {
    return (*a - *b);
}

//Function to calculate total head movement for FCFS algorithm
int FCFS(int requests[], int num_requests, int initial_position) {
    int head = initial_position;
    int total_head_movement = 0;

    for (int i = 0; i < num_requests; i++) {
        total_head_movement += abs(head - requests[i]);
        head = requests[i];
    }
    return total_head_movement;
}

//Function to calculate total head movement for SCAN algorithm
int SCAN(int requests[], int num_requests, int initial_position, int num_cylinders) {
    int head = initial_position;
    int total_head_movement = 0;

    //Separate requests into lower and higher than current position
    int lower[NUM_REQUESTS], higher[NUM_REQUESTS];
    int lower_count = 0, higher_count = 0;

    for (int i = 0; i < num_requests; i++) {
        if (requests[i] < head) {
            lower[lower_count++] = requests[i];
        } else {
            higher[higher_count++] = requests[i];
        }
    }

    //Sort both lower and higher requests
    qsort(lower, lower_count, sizeof(int), (int (*)(const void *, const void *)) compare);
    qsort(higher, higher_count, sizeof(int), (int (*)(const void *, const void *)) compare);

    //Sweep to the leftmost request first, then go to the rightmost request
    total_head_movement += abs(head - lower[0]); // Move to the leftmost request
    total_head_movement += lower[lower_count - 1]; // Go to the leftmost position
    total_head_movement += (higher[higher_count - 1] - lower[lower_count - 1]); // Sweep all the way to the rightmost request

    return total_head_movement;
}

//Function to calculate total head movement for C-SCAN algorithm
int C_SCAN(int requests[], int num_requests, int initial_position, int num_cylinders) {
    int head = initial_position;
    int total_head_movement = 0;

    //Separate requests into lower and higher than current position
    int lower[NUM_REQUESTS], higher[NUM_REQUESTS];
    int lower_count = 0, higher_count = 0;

    for (int i = 0; i < num_requests; i++) {
        if (requests[i] < head) {
            lower[lower_count++] = requests[i];
        } else {
            higher[higher_count++] = requests[i];
        }
    }

    //Sort both lower and higher requests
    qsort(lower, lower_count, sizeof(int), (int (*)(const void *, const void *)) compare);
    qsort(higher, higher_count, sizeof(int), (int (*)(const void *, const void *)) compare);

    //Go to the highest request first, then wrap around to the lowest request
    total_head_movement += abs(head - higher[0]);
    total_head_movement += (num_cylinders - 1 - higher[higher_count - 1]);
    total_head_movement += (num_cylinders - 1 - lower[0]);

    return total_head_movement;
}

//Function to generate random disk requests
void generateRequests(int requests[], int num_requests) {
    for (int i = 0; i < num_requests; i++) {
        requests[i] = rand() % NUM_CYLINDERS; // Random cylinder number
    }
}

void printResults(int fcfs_movement, int scan_movement, int cscan_movement) {
    printf("Algorithm  | Total Head Movement\n");
    printf("-----------------------------\n");
    printf("FCFS       | %d\n", fcfs_movement);
    printf("SCAN       | %d\n", scan_movement);
    printf("C-SCAN     | %d\n", cscan_movement);
}

int main(int argc, char *argv[]) {
    //Check if the user has provided the initial disk head position as a command-line argument
    if (argc != 2) {
        printf("Usage: %s <initial_disk_head_position>\n", argv[0]);
        return 1;
    }

    //Read the initial disk head position from the command line argument
    int initial_position = atoi(argv[1]);
    int requests[NUM_REQUESTS];
  
    srand(time(0));

    //Generate a random series of 1000 disk requests
    generateRequests(requests, NUM_REQUESTS);

    //Calculate head movements for each algorithm
    int fcfs_movement = FCFS(requests, NUM_REQUESTS, initial_position);
    int scan_movement = SCAN(requests, NUM_REQUESTS, initial_position, NUM_CYLINDERS);
    int cscan_movement = C_SCAN(requests, NUM_REQUESTS, initial_position, NUM_CYLINDERS);

    printResults(fcfs_movement, scan_movement, cscan_movement);

    return 0;
}
