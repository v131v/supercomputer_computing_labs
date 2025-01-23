#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include "../lib/utils.h"

using namespace std;

int compute_dot_product(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    int dotProduct = 0;

    #pragma omp parallel for reduction(+:dotProduct)
    for (size_t i = 0; i < vec1.size(); ++i) {
        dotProduct += vec1[i] * vec2[i];
    }

    return dotProduct;
}

int main() {
    string filename;
    int size;

    cout << "Путь до векторов: ";
    cin >> filename;

    vector<int> vec1, vec2;

    double readTime = 0.0, computeTime = 0.0;

    #pragma omp sections
    {
        #pragma omp section
        {
            double start = omp_get_wtime();
            read_vectors_from_file(filename, vec1, vec2);
            readTime = omp_get_wtime() - start;
        }
    }

    int result = 0;
    #pragma omp sections
    {
        #pragma omp section
        {
            double start = omp_get_wtime();
            result = compute_dot_product(vec1, vec2);
            computeTime = omp_get_wtime() - start;
        }
    }

    cout << "Скалярное произведение: " << result << endl;
    cout << "Время чтения данных: " << readTime << " секунд" << endl;
    cout << "Время вычисления: " << computeTime << " секунд" << endl;

    return 0;
}
