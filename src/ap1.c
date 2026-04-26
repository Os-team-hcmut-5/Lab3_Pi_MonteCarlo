#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mt19937-64.c"

void init_genrand64(unsigned long long seed);
double genrand64_real2(void);


double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main() {
    // 100 Million points 
    long long num_points = 100000000LL; 
    long long count = 0;
    
    // Mersenne Twister uses a 64-bit seed
    unsigned long long seed = 42ULL; 

    // Seed the Mersenne Twister 
    init_genrand64(seed);

    printf("Total Points: %lld\n", num_points);
    printf("--------------------------------------------------------------------------------\n");

    double start_time = get_time();
    
    for (long long i = 0; i < num_points; i++) {

        double x = genrand64_real2() * 2.0 - 1.0;
        double y = genrand64_real2() * 2.0 - 1.0;
        
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