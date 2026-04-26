#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "mt19937-64.c" // Including directly (Method 2)

// Tell the compiler these functions exist
void init_genrand64(unsigned long long seed);
double genrand64_real2(void);

typedef struct {
    long long num_points;
    unsigned long long seed; // Updated to 64-bit seed
    long long points_inside;
} ThreadData;

void* calculate_pi_worker(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long local_count = 0;
    
    // 1. Seed the Mersenne Twister strictly for this thread
    init_genrand64(data->seed);
    
    for (long long i = 0; i < data->num_points; i++) {
        // 2. genrand64_real2() returns a float in [0.0, 1.0)
        // Multiply by 2.0 and subtract 1.0 to get exactly [-1.0, 1.0)
        double x = genrand64_real2() * 2.0 - 1.0;
        double y = genrand64_real2() * 2.0 - 1.0;
        
        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }
    
    data->points_inside = local_count;
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
    printf("%-10s | %-12s | %-15s | %-10s | %-10s\n", 
           "Threads", "Pi Estimate", "Time (seconds)", "Speedup", "Points/Thread");
    printf("--------------------------------------------------------------------------------\n");

    for (int t = 0; t < num_configs; t++) {
        int N = thread_counts[t];
        long long points_per_thread = TOTAL_POINTS / N;
        long long remaining_points = TOTAL_POINTS % N;

        pthread_t threads[N];
        ThreadData thread_data[N];

        double start_time = get_time();

        for (int i = 0; i < N; i++) {
            thread_data[i].num_points = points_per_thread + (i == 0 ? remaining_points : 0);
            
            // Assign a unique 64-bit seed to each thread
            thread_data[i].seed = 42ULL + i; 
            
            thread_data[i].points_inside = 0;
            
            pthread_create(&threads[i], NULL, calculate_pi_worker, &thread_data[i]);
        }

        long long total_points_inside = 0;
        for (int i = 0; i < N; i++) {
            pthread_join(threads[i], NULL);
            total_points_inside += thread_data[i].points_inside;
        }

        double end_time = get_time();
        double execution_time = end_time - start_time;
        double pi_estimate = 4.0 * ((double)total_points_inside / TOTAL_POINTS);

        if (N == 1) {
            t_single = execution_time;
        }

        double speedup = t_single / execution_time;

        printf("%-10d | %-12.6f | %-15.6f | %-10.2fx | %-10lld\n", 
               N, pi_estimate, execution_time, speedup, points_per_thread);
    }

    printf("--------------------------------------------------------------------------------\n");
    return 0;
}