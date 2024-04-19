#include <iostream>
#include <cstdlib>
#include <ctime>
#include "omp.h"

using namespace std;

const int rows = 1000;
const int cols = 1000;

int arr[rows][cols];

void init_arr();
long long part_sum(int, int, int);
void part_min(int, int, int, int&, int&);

int main() {
    init_arr();

    omp_set_nested(1);
    double t1 = omp_get_wtime();
    int min_row = 0;
    int min_sum = INT_MAX;
#pragma omp parallel sections
    {
#pragma omp section
        {
            double section_t1 = omp_get_wtime();
            part_min(0, rows, 1, min_row, min_sum);
            double section_t2 = omp_get_wtime();
            cout << "Time for min 1 = " << section_t2 - section_t1 << " seconds" << endl;
        }

#pragma omp section
        {
            double section_t1 = omp_get_wtime();
            cout << "sum 1 = " << part_sum(0, rows, 1) << endl;
            double section_t2 = omp_get_wtime();
            cout << "Time for sum 1 = " << section_t2 - section_t1 << " seconds" << endl;
        }
    }
    double t2 = omp_get_wtime();

    cout << "Total time = " << t2 - t1 << " seconds" << endl;
    cout << "Row with minimal sum: " << min_row << ", Sum: " << min_sum << endl;
    return 0;
}

void init_arr() {
    srand(time(NULL));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            arr[i][j] = rand() % 100;
        }
    }
}

long long part_sum(int start_row, int end_row, int num_threads) {
    long long total_sum = 0;
#pragma omp parallel for reduction(+:total_sum) num_threads(num_threads)
    for (int i = start_row; i < end_row; i++) {
        long long row_sum = 0;
        for (int j = 0; j < cols; j++) {
            row_sum += arr[i][j];
        }
        total_sum += row_sum;
    }
    return total_sum;
}

void part_min(int start_row, int end_row, int num_threads, int& min_row, int& min_sum) {
#pragma omp parallel for num_threads(num_threads)
    for (int i = start_row; i < end_row; i++) {
        long long row_sum = 0;
        for (int j = 0; j < cols; j++) {
            row_sum += arr[i][j];
        }
#pragma omp critical
        {
            if (row_sum < min_sum) {
                min_sum = row_sum;
                min_row = i;
            }
        }
        
    }
}
