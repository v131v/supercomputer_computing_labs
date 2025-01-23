#include <iostream>
#include <limits>
#include <vector>
#include <omp.h>
#include "../lib/utils.h"

using namespace std;

// Функция для поиска максимального среди минимальных элементов строк матрицы
int find_max_of_row_mins(const vector<vector<int>>& matrix, bool use_nested_parallelism) {
    int max_min = numeric_limits<int>::min(); // Глобальный максимум
    int rows = matrix.size();
    int cols = matrix[0].size();

    // Устанавливаем количество уровней параллелизма
    omp_set_max_active_levels(use_nested_parallelism ? 2 : 1);

    #pragma omp parallel for reduction(max : max_min)
    for (int i = 0; i < rows; ++i) {
        int row_min = numeric_limits<int>::max();

        // Вложенный параллельный цикл (активен только при включении вложенного параллелизма)
        #pragma omp parallel for reduction(min : row_min) if (omp_get_active_level() < omp_get_max_active_levels())
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] < row_min) {
                row_min = matrix[i][j];
            }
        }

        // Обновление глобального максимума
        if (row_min > max_min) {
            max_min = row_min;
        }
    }

    return max_min;
}

int main() {
    int m, n;

    // Ввод данных
    cout << "Введите количество строк матрицы (m): ";
    cin >> m;
    cout << "Введите количество столбцов матрицы (n): ";
    cin >> n;

    const int min_value = -100;
    const int max_value = 100;

    // Генерация случайной матрицы
    vector<vector<int>> matrix = generate_matrix(m, n, min_value, max_value);

    // Замер времени выполнения без вложенного параллелизма
    double time_without_nested = measure_time([&]() {
        int result = find_max_of_row_mins(matrix, false);
        cout << "Максимум без вложенного параллелизма: " << result << endl;
    });

    // Замер времени выполнения с вложенным параллелизмом
    double time_with_nested = measure_time([&]() {
        int result = find_max_of_row_mins(matrix, true);
        cout << "Максимум с вложенным параллелизмом: " << result << endl;
    });

    // Вывод времени выполнения
    cout << "Время без вложенного параллелизма: " << time_without_nested << " секунд." << endl;
    cout << "Время с вложенным параллелизмом: " << time_with_nested << " секунд." << endl;

    return 0;
}
