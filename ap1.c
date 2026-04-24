#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main() {
    // 100 Million points 
    long long num_points = 100000000LL; 
    long long count = 0;
    unsigned int seed = 42; 

    printf("Total Points: %lld\n", num_points);
    printf("--------------------------------------------------------------------------------\n");

    double start_time = get_time();
    
    // Calculate on the fly (no massive arrays in memory)
    for (long long i = 0; i < num_points; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        
        if (x * x + y * y <= 1.0) {
            count++;
        }
    }
    
    double end_time = get_time();
    double execution_time = end_time - start_time;

    double pi_approx = 4.0 * count / num_points;
    
    printf("Pi Estimate: %f\n", pi_approx);
    printf("Execution Time: %f seconds\n", execution_time);
    printf("--------------------------------------------------------------------------------\n");

    return 0;
}