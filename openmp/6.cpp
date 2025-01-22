#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include "../lib/utils.h"

using namespace std;

// Функция для генерации случайных чисел в цикле с неравномерной нагрузкой
void generate_workload(vector<int>& data, int size) {
    srand(time(0));  // Инициализация генератора случайных чисел
    for (int i = 0; i < size; ++i) {
        if (i % 3 == 0) {
            data[i] = rand() % 1000;  // Большие значения
        } else {
            data[i] = rand() % 10;  // Маленькие значения
        }
    }
}

int main() {
    int size = 1000000;  // Размер массива
    vector<int> data(size);

    // Генерация нагрузки с неравномерными значениями
    generate_workload(data, size);

    // Замер времени для различных типов распределения итераций
    for (const char* schedule_type : {"static", "dynamic", "guided"}) {
        cout << "Режим распределения итераций: " << schedule_type << endl;

        // Измерение времени с использованием разных схем распределения итераций
        double elapsed_time = measure_time([&]() {
            if (string(schedule_type) == "static") {
                #pragma omp parallel for schedule(static)
                for (int i = 0; i < size; ++i) {
                    data[i] += data[i];
                }
            } else if (string(schedule_type) == "dynamic") {
                #pragma omp parallel for schedule(dynamic)
                for (int i = 0; i < size; ++i) {
                    data[i] += data[i];
                }
            } else if (string(schedule_type) == "guided") {
                #pragma omp parallel for schedule(guided)
                for (int i = 0; i < size; ++i) {
                    data[i] += data[i];
                }
            }
        });
        cout << "Время работы (режим " << schedule_type << "): " << elapsed_time << " секунд." << endl;
    }

    return 0;
}
