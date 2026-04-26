#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "mt19937-64.c" 

void init_genrand64(unsigned long long seed);
double genrand64_real2(void);

long long global_points_inside = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    long long num_points;
    unsigned long long seed; // Updated to 64-bit for MT
} ThreadData;

void* calculate_pi_worker(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
    init_genrand64(data->seed);
    
    for (long long i = 0; i < data->num_points; i++) {
        double x = genrand64_real2() * 2.0 - 1.0;
        double y = genrand64_real2() * 2.0 - 1.0;
        
        if (x * x + y * y <= 1.0) {
            pthread_mutex_lock(&mutex);
            global_points_inside++;
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main() {
    const long long TOTAL_POINTS = 100000000LL; 
    
    int thread_counts[] = {1, 2, 4, 8, 16, 32, 64, 100};
    int num_configs = sizeof(thread_counts) / sizeof(thread_counts[0]);
    double t_single = 0.0;

    printf("Total Points: %lld\n", TOTAL_POINTS);
    printf("--------------------------------------------------------------------------------\n");
    printf("%-10s | %-12s | %-15s | %-10s\n", 
           "Threads", "Pi Estimate", "Time (seconds)", "Speedup");
    printf("--------------------------------------------------------------------------------\n");

    for (int t = 0; t < num_configs; t++) {
        int N = thread_counts[t];
        long long points_per_thread = TOTAL_POINTS / N;
        long long remaining_points = TOTAL_POINTS % N;

        pthread_t threads[N];
        ThreadData thread_data[N];

        // Reset global counter for each test run
        global_points_inside = 0;

        double start_time = get_time();

        for (int i = 0; i < N; i++) {
            thread_data[i].num_points = points_per_thread + (i == 0 ? remaining_points : 0);
            
            // Unique 64-bit seed per thread
            thread_data[i].seed = 42ULL + i;
            
            pthread_create(&threads[i], NULL, calculate_pi_worker, &thread_data[i]);
        }

        for (int i = 0; i < N; i++) {
            pthread_join(threads[i], NULL);
        }

        double end_time = get_time();
        double execution_time = end_time - start_time;
        double pi_estimate = 4.0 * ((double)global_points_inside / TOTAL_POINTS);

        if (N == 1) {
            t_single = execution_time;
        }
        double speedup = t_single / execution_time;

        printf("%-10d | %-12.6f | %-15.6f | %-10.2fx\n", 
               N, pi_estimate, execution_time, speedup);
    }

    printf("--------------------------------------------------------------------------------\n");
    pthread_mutex_destroy(&mutex);
    return 0;
}