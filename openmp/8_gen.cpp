#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../lib/utils.h"

using namespace std;

int main() {
    string filename;
    int vector_size;      // Размер каждого вектора

	cout << "Путь до файла с векторами: ";
	cin >> filename;

	cout << "Размер векторов: ";
	cin >> vector_size;

    // Инициализация генератора случайных чисел
    srand(time(0));

    generate_vectors_file(filename, vector_size);

    return 0;
}
