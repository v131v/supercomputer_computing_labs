#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "omp.h"
#include "../lib/utils.h"

using namespace std;

// Функция для поиска максимального значения среди минимальных элементов строк
int find_max_of_min_elements(const vector<vector<int>>& matrix) {
    int max_of_min = numeric_limits<int>::min();

    #pragma omp parallel for reduction(max:max_of_min)
    for (int i = 0; i < matrix.size(); ++i) {
        int min_in_row = *min_element(matrix[i].begin(), matrix[i].end());
        max_of_min = max(max_of_min, min_in_row);
    }

    return max_of_min;
}

int main() {
    int m, n;

    // Ввод данных
    cout << "Введите количество строк матрицы (m): ";
    cin >> m;
    cout << "Введите количество столбцов матрицы (n): ";
    cin >> n;

    // Генерация случайной матрицы
    srand(time(0));
    vector<vector<int>> matrix = generate_matrix(m, n, 0, 100);

    // Тестирование с разным количеством потоков
    int max_threads = omp_get_max_threads();
    cout << "Максимальное количество потоков: " << max_threads << "\n";

    // Последовательный расчет для вычисления ускорения
    double serial_time = measure_time([&]() {
        int result = find_max_of_min_elements(matrix);
        cout << "Результат (последовательно): " << result << endl;
    });
    cout << "Время (последовательно): " << serial_time << " секунд\n";

    // Проведение эксперимента с разным количеством потоков
    for (int num_threads = 1; num_threads <= max_threads; ++num_threads) {
        omp_set_num_threads(num_threads);

        double parallel_time = measure_time([&]() {
            int result = find_max_of_min_elements(matrix);
            cout << "Результат (параллельно, потоки: " << num_threads << "): " << result << endl;
        });

        double speedup = serial_time / parallel_time; // Расчёт ускорения
        cout << "Время (параллельно, потоки: " << num_threads << "): " << parallel_time << " секунд\n";
        cout << "Ускорение: " << speedup << "\n\n";
    }

    return 0;
}
