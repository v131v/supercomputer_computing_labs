#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>
#include <ctime>

using namespace std;

void multiply_matrices_latch(int* A, int* B, int* C, int n, int rows_per_proc, int rank, int size) {
    int start_row = rank * rows_per_proc;

    // Убедитесь, что данные на всех процессах корректны
    vector<int> local_A(rows_per_proc * n);
    vector<int> local_C(rows_per_proc * n, 0);

    // Распределяем строки матрицы A между процессами
    MPI_Scatter(A, rows_per_proc * n, MPI_INT, local_A.data(), rows_per_proc * n, MPI_INT, 0, MPI_COMM_WORLD);

    // Передаем матрицу B всем процессам
    MPI_Bcast(B, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    // Умножаем локальные части
    for (int i = 0; i < rows_per_proc; ++i) {
        for (int j = 0; j < n; ++j) {
            local_C[i * n + j] = 0;
            for (int k = 0; k < n; ++k) {
                local_C[i * n + j] += local_A[i * n + k] * B[k * n + j];
            }
        }
    }

    // Собираем результаты в матрицу C
    MPI_Gather(local_C.data(), rows_per_proc * n, MPI_INT, C, rows_per_proc * n, MPI_INT, 0, MPI_COMM_WORLD);
}

int main(int argc, char* argv[]) {
    int rank, size;
    int n = 1000; // Размер матрицы
    int rows_per_proc;
    vector<int> A, B, C;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    rows_per_proc = n / size;

    if (rank == 0) {
        A.resize(n * n);
        B.resize(n * n);
        C.resize(n * n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                A[i * n + j] = rand() % 100;
                B[i * n + j] = rand() % 100;
            }
        }
    }

    double start_time = MPI_Wtime();

    multiply_matrices_latch(A.data(), B.data(), C.data(), n, rows_per_proc, rank, size);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        cout << "Matrix multiplication (latch algorithm) completed." << endl;
        cout << "Time taken: " << end_time - start_time << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
