#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <string>
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

// Чтение векторов из файла
void read_vectors_from_file(const std::string& filename, std::vector<int>& vec1, std::vector<int>& vec2);

// Функция для генерации файла с векторами
void generate_vectors_file(const std::string& filename, int size);

int get_random_number(int min, int max);

#endif
