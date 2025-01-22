#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <vector>

// Функция-обёртка для замера времени
double measure_time(const std::function<void()>& func);

std::vector<int> generate_random_vector(int n);

std::vector<std::vector<int>> generate_matrix(int m, int n, int min_val, int max_val);

// Функция для генерации ленточной матрицы
std::vector<std::vector<int>> generate_banded_matrix(int m, int n, int bandwidth, int min_val = 0, int max_val = 100);

// Функция для генерации верхнетреугольной матрицы
std::vector<std::vector<int>> generate_upper_triangular_matrix(int m, int n, int min_val = 0, int max_val = 100);

// Функция для поиска максимального значения среди минимальных элементов строк
int find_max_of_min_elements(const std::vector<std::vector<int>>& matrix);

// Функция для суммирования элементов вектора
int sum_elements(const std::vector<int>& data);

#endif
