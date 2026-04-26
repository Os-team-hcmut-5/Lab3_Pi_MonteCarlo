#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "mt19937-64.c" // Import your thread-safe Mersenne Twister

// Tell the compiler these MT functions exist
void init_genrand64(unsigned long long seed);
double genrand64_real2(void);

long long global_points_inside = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    long long num_points;
    unsigned long long seed; // Updated to 64-bit for Mersenne Twister
    long long sync_freq; 
} ThreadData;

void* calculate_pi_worker(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
    // Seed the Mersenne Twister strictly for this thread
    init_genrand64(data->seed);
    
    long long local_batch_inside = 0;

    for (long long i = 0; i < data->num_points; i++) {
        // Generate high-precision floats mapped to [-1.0, 1.0)
        double x = genrand64_real2() * 2.0 - 1.0;
        double y = genrand64_real2() * 2.0 - 1.0;
        
        if (x * x + y * y <= 1.0) {
            local_batch_inside++;
        }

        // Synchronization Frequency Logic
        if ((i + 1) % data->sync_freq == 0) {
            pthread_mutex_lock(&mutex);
            global_points_inside += local_batch_inside;
            pthread_mutex_unlock(&mutex);
            local_batch_inside = 0;
        }
    }
    
    // Catch any remaining points after the loop finishes
    if (local_batch_inside > 0) {
        pthread_mutex_lock(&mutex);
        global_points_inside += local_batch_inside;
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void run_experiment(long long total_points, int* threads_arr, int num_threads, long long sync_freq, const char* label) {
    double t_single = 0.0;

    printf("\n=== Experiment: %s (Update every %lld iterations) ===\n", label, sync_freq);
    printf("--------------------------------------------------------------------------------\n");
    printf("%-10s | %-12s | %-15s | %-10s\n", "Threads", "Pi Estimate", "Time (seconds)", "Speedup");
    printf("--------------------------------------------------------------------------------\n");

    for (int t = 0; t < num_threads; t++) {
        int N = threads_arr[t];
        long long points_per_thread = total_points / N;
        long long remaining_points = total_points % N;

        pthread_t threads[N];
        ThreadData thread_data[N];

        global_points_inside = 0; 
        double start_time = get_time();

        for (int i = 0; i < N; i++) {
            thread_data[i].num_points = points_per_thread + (i == 0 ? remaining_points : 0);
            
            // Assign a unique 64-bit seed to each thread
            thread_data[i].seed = 42ULL + i;
            
            // If sync_freq is 0, it means we only sync once at the very end (Local Accumulation)
            thread_data[i].sync_freq = (sync_freq == 0) ? thread_data[i].num_points : sync_freq;
            
            pthread_create(&threads[i], NULL, calculate_pi_worker, &thread_data[i]);
        }

        for (int i = 0; i < N; i++) {
            pthread_join(threads[i], NULL);
        }

        double end_time = get_time();
        double execution_time = end_time - start_time;
        double pi_estimate = 4.0 * ((double)global_points_inside / total_points);

        if (N == 1) {
            t_single = execution_time;
        }

        double speedup = t_single / execution_time;

        printf("%-10d | %-12.6f | %-15.6f | %-10.2fx\n", N, pi_estimate, execution_time, speedup);
    }
    printf("--------------------------------------------------------------------------------\n");
}

int main() {
    // 100 Million points
    const long long TOTAL_POINTS = 100000000LL; 
    int thread_counts[] = {1, 2, 4, 8, 16, 32, 64, 100};
    int num_configs = sizeof(thread_counts) / sizeof(thread_counts[0]);

    printf("Total Points: %lld\n", TOTAL_POINTS);

    run_experiment(TOTAL_POINTS, thread_counts, num_configs, 1, "Shared Accumulation - Extreme Contention");
    
    run_experiment(TOTAL_POINTS, thread_counts, num_configs, 10000, "Shared Accumulation - Moderate Contention");
    
    run_experiment(TOTAL_POINTS, thread_counts, num_configs, 0, "Local Accumulation (Near Ideal)");

    pthread_mutex_destroy(&mutex);
    return 0;
}