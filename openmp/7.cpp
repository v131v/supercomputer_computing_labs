#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <mutex>
#include "../lib/utils.h"

using namespace std;

int main() {
    int size = 1000000;  // Размер массива
    vector<int> data = generate_random_vector(size);

    // 1. Использование атомарных операций
    double time_atomic = measure_time([&]() {
        int total_sum = 0;
        #pragma omp parallel for atomic
        for (int i = 0; i < size; ++i) {
            total_sum += data[i];
        }
    });
    cout << "Время работы (атомарные операции): " << time_atomic << " секунд." << endl;

    // 2. Использование критической секции
    double time_critical = measure_time([&]() {
        int total_sum = 0;
        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            #pragma omp critical
            {
                total_sum += data[i];
            }
        }
    });
    cout << "Время работы (критическая секция): " << time_critical << " секунд." << endl;

    // 3. Использование замков (mutex)
    mutex mtx;
    double time_mutex = measure_time([&]() {
        int total_sum = 0;
        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            lock_guard<mutex> lock(mtx);
            total_sum += data[i];
        }
    });
    cout << "Время работы (замки - mutex): " << time_mutex << " секунд." << endl;

    // 4. Использование редукции OpenMP
    double time_reduction = measure_time([&]() {
        int total_sum = 0;
        #pragma omp parallel for reduction(+:total_sum)
        for (int i = 0; i < size; ++i) {
            total_sum += data[i];
        }
    });
    cout << "Время работы (редукция OpenMP): " << time_reduction << " секунд." << endl;

    return 0;
}
