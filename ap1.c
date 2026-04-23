#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
} Point;

// Hàm đếm số điểm nằm trong hình tròn
int countInCircle(Point *points, int num_points) {
    int count = 0;
    for (int i = 0; i < num_points; i++) {
        // Tính x^2 + y^2
        double distance_sq = points[i].x * points[i].x + points[i].y * points[i].y;
        
        if (distance_sq <= 1.0) {
            count++;
        }
    }
    return count;
}

int main() {
    // 10 triệu điểm
    long long num_points = 1e8; 
    
    Point *points = (Point *)malloc(num_points * sizeof(Point));
    
    if (points == NULL) {
        fprintf(stderr, "Không đủ bộ nhớ!\n");
        return 1;
    }

    unsigned int seed = 42; 
    srand(seed);

    // Tạo dữ liệu
    for (int i = 0; i < num_points; i++) {
        points[i].x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        points[i].y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
    }

    // Gọi hàm tính toán
    int inside_count = countInCircle(points, num_points);

    // Hiển thị kết quả
    // printf("Tong so diem: %d\n", num_points);
    // printf("So diem nam trong hinh tron: %d\n", inside_count);
    
    // Tính xấp xỉ số Pi: Pi = 4 * (inside / total)
    double pi_approx = 4.0 * inside_count / num_points;
    printf("Gia tri xap xi cua Pi: %f\n", pi_approx);

    free(points);
    return 0;
}

