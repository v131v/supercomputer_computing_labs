#include <iostream>
#include <functional>
#include <cmath>
#include <omp.h>
#include "../lib/utils.h"

using namespace std;

// Пример функции для интегрирования: f(x) = x^2
double f(double x) {
    return x * x;
}

// Метод прямоугольников для вычисления определённого интеграла
double rectangle_integral(double (*func)(double), double a, double b, int n) {
    double delta_x = (b - a) / n;
    double integral = 0.0;

    #pragma omp parallel for reduction(+:integral)
    for (int i = 0; i < n; ++i) {
        double x = a + i * delta_x;
        integral += func(x) * delta_x;
    }

    return integral;
}

int main() {
    double a, b;
    int n;

    // Ввод данных
    cout << "Введите границы интегрирования (a, b): ";
    cin >> a >> b;
    cout << "Введите количество разбиений n: ";
    cin >> n;

    // Тестирование с разным количеством потоков
    int max_threads = omp_get_max_threads();
    cout << "Максимальное количество потоков: " << max_threads << "\n";
    cout << "Размер задачи (n): " << n << "\n";

    // Последовательный расчет для вычисления ускорения
    double serial_time = 0.0;
    serial_time = measure_time([&]() {
        double result = rectangle_integral(f, a, b, n);
        cout << "Результат интегрирования (последовательно): " << result << endl;
    });
    cout << "Время (последовательно): " << serial_time << " секунд\n";

    // Проведение эксперимента с разным количеством потоков
    for (int num_threads = 1; num_threads <= max_threads; ++num_threads) {
        omp_set_num_threads(num_threads);

        double parallel_time = measure_time([&]() {
            double result = rectangle_integral(f, a, b, n);
            cout << "Результат интегрирования (параллельно, потоки: " << num_threads << "): " << result << endl;
        });

        double speedup = serial_time / parallel_time; // Расчёт ускорения
        cout << "Время (параллельно, потоки: " << num_threads << "): " << parallel_time << " секунд\n";
        cout << "Ускорение: " << speedup << "\n\n";
    }

    return 0;
}
