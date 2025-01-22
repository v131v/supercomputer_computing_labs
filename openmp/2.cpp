#include <iostream>
#include <vector>
#include <omp.h>
#include "../lib/utils.h" // Подключение заголовочного файла

using namespace std;

int main() {
    // Ввод размера векторов
    int n;
    cout << "Введите размер векторов: ";
    cin >> n;

    // Генерация случайных векторов
    vector<int> vec1 = generate_random_vector(n);
    vector<int> vec2 = generate_random_vector(n);

    int dot_product_reduction = 0;
    int dot_product_no_reduction = 0;

    // --- С редукцией ---
    double time_with_reduction = measure_time([&]() {
        dot_product_reduction = 0;
        #pragma omp parallel for reduction(+:dot_product_reduction)
        for (int i = 0; i < n; ++i) {
            dot_product_reduction += vec1[i] * vec2[i];
        }
    });

    cout << "[С редукцией] Скалярное произведение: " << dot_product_reduction
              << ", Время: " << time_with_reduction << " секунд\n";

    // --- Без редукции ---
    double time_without_reduction = measure_time([&]() {
        dot_product_no_reduction = 0;
        #pragma omp parallel
        {
            int local_dot_product = 0;

            #pragma omp for
            for (int i = 0; i < n; ++i) {
                local_dot_product += vec1[i] * vec2[i];
            }

            #pragma omp critical
            {
                dot_product_no_reduction += local_dot_product;
            }
        }
    });

    cout << "[Без редукции] Скалярное произведение: " << dot_product_no_reduction
              << ", Время: " << time_without_reduction << " секунд\n";

    return 0;
}
