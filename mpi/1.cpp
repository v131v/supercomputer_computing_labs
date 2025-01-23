#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    int n = 10000000; // Количество элементов в векторе

    vector<int> vec;
    int local_max, global_max;

    // Инициализация MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Генерация вектора на процессе с рангом 0
    if (rank == 0) {
        vec.resize(n);
        for (int i = 0; i < n; ++i) {
            vec[i] = rand() % 1000; // Заполнение случайными числами
        }
    }

    // Разбиение вектора между процессами
    int local_n = n / size; // Количество элементов на каждый процесс
    vector<int> local_vec(local_n);

    // Рассылка частей вектора каждому процессу
    MPI_Scatter(vec.data(), local_n, MPI_INT, local_vec.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Начало замера времени
    double start_time = MPI_Wtime();

    // Каждый процесс находит свой локальный максимум
    local_max = local_vec[0];
    for (int i = 1; i < local_n; ++i) {
        if (local_vec[i] > local_max) {
            local_max = local_vec[i];
        }
    }

    // Сбор локальных максимумов на процесс 0
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // Завершение замера времени
    double end_time = MPI_Wtime();

    // Процесс 0 выводит результат
    if (rank == 0) {
		cout << "Vector size: " << n << endl;
        cout << "Global Max value: " << global_max << endl;
        cout << "Time taken: " << end_time - start_time << " seconds" << endl;
    }

    // Завершаем работу с MPI
    MPI_Finalize();
    return 0;
}
