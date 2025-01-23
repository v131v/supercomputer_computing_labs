#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>
#include <ctime>
#include <cmath>

using namespace std;

void multiply_matrices_latch(int* A, int* B, int* C, int n, int rows_per_proc, int rank, int size);

void multiply_matrices_fox(int* A, int* B, int* C, int n, int rows_per_proc, int rank, int size) {
    int sqrt_p = sqrt(size); // Предполагаем, что количество процессов - квадрат числа
    int block_size = n / sqrt_p;

    // Индекс процессора в 2D пространстве
    int row_rank = rank / sqrt_p;
    int col_rank = rank % sqrt_p;

    // Разбиение матриц на блоки
    vector<int> local_A(block_size * block_size);
    vector<int> local_B(block_size * block_size);
    vector<int> local_C(block_size * block_size, 0);

    // Распределяем блоки матриц
    MPI_Scatter(A, block_size * block_size, MPI_INT, local_A.data(), block_size * block_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, block_size * block_size, MPI_INT, local_B.data(), block_size * block_size, MPI_INT, 0, MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    // Алгоритм Фокса
    for (int stage = 0; stage < sqrt_p; ++stage) {
        // Множим локальные блоки
        for (int i = 0; i < block_size; ++i) {
            for (int j = 0; j < block_size; ++j) {
                for (int k = 0; k < block_size; ++k) {
                    local_C[i * block_size + j] += local_A[i * block_size + k] * local_B[k * block_size + j];
                }
            }
        }

        // Перемещаем блоки
        MPI_Sendrecv_replace(local_A.data(), block_size * block_size, MPI_INT,
                             (row_rank + 1) % sqrt_p * sqrt_p + col_rank, 0,
                             (row_rank - 1 + sqrt_p) % sqrt_p * sqrt_p + col_rank, 0,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv_replace(local_B.data(), block_size * block_size, MPI_INT,
                             row_rank * sqrt_p + (col_rank + 1) % sqrt_p, 0,
                             row_rank * sqrt_p + (col_rank - 1 + sqrt_p) % sqrt_p, 0,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    double end_time = MPI_Wtime();

    MPI_Gather(local_C.data(), block_size * block_size, MPI_INT, C, block_size * block_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Matrix multiplication completed with Fox's algorithm" << endl;
        cout << "Time taken: " << end_time - start_time << " seconds" << endl;
    }
}

void multiply_matrices_latch(int* A, int* B, int* C, int n, int rows_per_proc, int rank, int size) {
    int start_row = rank * rows_per_proc;
    for (int i = start_row; i < start_row + rows_per_proc; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i * n + j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
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

    // Выбор алгоритма
    if (argc > 1 && string(argv[1]) == "fox") {
        multiply_matrices_fox(A.data(), B.data(), C.data(), n, rows_per_proc, rank, size);
    } else {
        multiply_matrices_latch(A.data(), B.data(), C.data(), n, rows_per_proc, rank, size);
    }

    double end_time = MPI_Wtime();

    if (rank == 0) {
        cout << "Total time taken: " << end_time - start_time << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
