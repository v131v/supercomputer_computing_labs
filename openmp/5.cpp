#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include "../lib/utils.h"

using namespace std;

int main() {
    int m, n, bandwidth;

    // Ввод данных
    cout << "Введите количество строк матрицы (m): ";
    cin >> m;
    cout << "Введите количество столбцов матрицы (n): ";
    cin >> n;

    // Выбор типа матрицы
    int matrix_type;
    cout << "Выберите тип матрицы (1 - ленточная, 2 - верхнетреугольная): ";
    cin >> matrix_type;

    vector<vector<int>> matrix;

    if (matrix_type == 1) {
        cout << "Введите ширину полосы для ленточной матрицы: ";
        cin >> bandwidth;
        matrix = generate_banded_matrix(m, n, bandwidth);
    } else {
        matrix = generate_upper_triangular_matrix(m, n);
    }

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
