#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    int n = 100000000;  // Размер векторов

    vector<int> vec_a, vec_b;
    int local_sum = 0, global_sum = 0;

    // Инициализация MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Генерация векторов на процессе с рангом 0
    if (rank == 0) {
        vec_a.resize(n);
        vec_b.resize(n);
        for (int i = 0; i < n; ++i) {
            vec_a[i] = rand() % 1000;  // Заполнение случайными числами
            vec_b[i] = rand() % 1000;  // Заполнение случайными числами
        }
    }

    // Разбиение векторов между процессами
    int local_n = n / size;  // Количество элементов на каждый процесс
    vector<int> local_a(local_n), local_b(local_n);

    // Рассылка частей векторов каждому процессу
    MPI_Scatter(vec_a.data(), local_n, MPI_INT, local_a.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(vec_b.data(), local_n, MPI_INT, local_b.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Начало замера времени
    double start_time = MPI_Wtime();

    // Каждый процесс вычисляет локальное скалярное произведение
    for (int i = 0; i < local_n; ++i) {
        local_sum += local_a[i] * local_b[i];
    }

    // Сбор локальных сумм на процесс 0
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Завершение замера времени
    double end_time = MPI_Wtime();

    // Процесс 0 выводит результат
    if (rank == 0) {
		cout << "Vector size: " << n << endl;
        cout << "Scalar Product: " << global_sum << endl;
        cout << "Time taken: " << end_time - start_time << " seconds" << endl;
    }

    // Завершаем работу с MPI
    MPI_Finalize();
    return 0;
}
