#include <iostream>
#include <vector>
#include <limits> // Для min и max
#include <omp.h>
#include "../lib/utils.h" // Подключение заголовочного файла

using namespace std;

int main() {
    // Ввод размера вектора
    int n;
    cout << "Введите размер вектора: ";
    cin >> n;

    // Генерация случайного вектора
    vector<int> vec = generate_random_vector(n);

    int min_value_reduction = numeric_limits<int>::max();
    int max_value_reduction = numeric_limits<int>::min();

    // --- С редукцией ---
    double time_with_reduction = measure_time([&]() {
        #pragma omp parallel for reduction(min:min_value_reduction) reduction(max:max_value_reduction)
        for (int i = 0; i < n; ++i) {
            if (vec[i] < min_value_reduction) min_value_reduction = vec[i];
            if (vec[i] > max_value_reduction) max_value_reduction = vec[i];
        }
    });

    cout << "[С редукцией] " << time_with_reduction << " секунд\n";

    int min_value_no_reduction = numeric_limits<int>::max();
    int max_value_no_reduction = numeric_limits<int>::min();

    // --- Без редукции ---
    double time_without_reduction = measure_time([&]() {
        #pragma omp parallel
        {
            int local_min = numeric_limits<int>::max();
            int local_max = numeric_limits<int>::min();

            #pragma omp for
            for (int i = 0; i < n; ++i) {
                if (vec[i] < local_min) local_min = vec[i];
                if (vec[i] > local_max) local_max = vec[i];
            }

            #pragma omp critical
            {
                if (local_min < min_value_no_reduction) min_value_no_reduction = local_min;
                if (local_max > max_value_no_reduction) max_value_no_reduction = local_max;
            }
        }
    });

    cout << "[Без редукции] " << time_without_reduction << " секунд\n";

    return 0;
}
