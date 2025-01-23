#include <iostream>
#include <omp.h>
#include <limits>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "utils.h"

// Функция-обёртка для замера времени
double measure_time(const std::function<void()>& func) {
    double start_time = omp_get_wtime();
    func();
    double end_time = omp_get_wtime();
    return end_time - start_time;
}

std::vector<int> generate_random_vector(int n) {
    std::vector<int> vec(n);
    for (int i = 0; i < n; ++i) {
        vec[i] = rand() % 1000;
    }
    return vec;
}

std::vector<std::vector<int>> generate_matrix(int m, int n, int min_val, int max_val) {
    std::vector<std::vector<int>> matrix(m, std::vector<int>(n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = rand() % (max_val - min_val + 1) + min_val;
        }
    }
    return matrix;
}

// Функция для генерации ленточной матрицы
std::vector<std::vector<int>> generate_banded_matrix(int m, int n, int bandwidth, int min_val, int max_val) {
    std::vector<std::vector<int>> matrix(m, std::vector<int>(n, 0));

    for (int i = 0; i < m; ++i) {
        for (int j = std::max(0, i - bandwidth); j <= std::min(n - 1, i + bandwidth); ++j) {
            matrix[i][j] = rand() % (max_val - min_val + 1) + min_val;
        }
    }
    return matrix;
}

// Функция для генерации верхнетреугольной матрицы
std::vector<std::vector<int>> generate_upper_triangular_matrix(int m, int n, int min_val, int max_val) {
    std::vector<std::vector<int>> matrix(m, std::vector<int>(n, 0));

    for (int i = 0; i < m; ++i) {
        for (int j = i; j < n; ++j) {
            matrix[i][j] = rand() % (max_val - min_val + 1) + min_val;
        }
    }
    return matrix;
}

int find_max_of_min_elements(const std::vector<std::vector<int>>& matrix) {
    int max_of_min = std::numeric_limits<int>::min();

    #pragma omp parallel for reduction(max:max_of_min)
    for (int i = 0; i < matrix.size(); ++i) {
        int min_in_row = *std::min_element(matrix[i].begin(), matrix[i].end());
        max_of_min = std::max(max_of_min, min_in_row);
    }

    return max_of_min;
}

int sum_elements(const std::vector<int>& data) {
    int total_sum = 0;
    #pragma omp parallel for reduction(+:total_sum)
    for (int i = 0; i < data.size(); ++i) {
        total_sum += data[i];
    }
    return total_sum;
}

void generate_vectors_file(const std::string& filename, int size) {
    std::ofstream outFile(filename);

    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < size; ++i) {
        outFile << rand() % 100 << " ";
    }
    outFile << std::endl;

    for (int i = 0; i < size; ++i) {
        outFile << rand() % 100 << " ";
    }

    outFile.close();
}

void read_vectors_from_file(const std::string& filename, std::vector<int>& vec1, std::vector<int>& vec2) {
    std::ifstream inFile(filename);
    std::string line;

    if (std::getline(inFile, line)) {
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            vec1.push_back(value);
        }
    }

    if (std::getline(inFile, line)) {
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            vec2.push_back(value);
        }
    }

    inFile.close();
}

int get_random_number(int min, int max) {
    return min + rand() % (max - min + 1);
}
