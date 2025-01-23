#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>
#include <ctime>

using namespace std;

void measure_communication(int rank, int size, int n) {
    vector<char> message(n, 0);  // Сообщение размером n байт
    double start_time, end_time;

    // Замер времени передачи сообщения
    if (rank == 0) {
        // Процесс 0 отправляет сообщение, а процесс 1 принимает
        start_time = MPI_Wtime();
        MPI_Send(message.data(), n, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(message.data(), n, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end_time = MPI_Wtime();
    } else if (rank == 1) {
        // Процесс 1 принимает сообщение, а затем отправляет его обратно
        MPI_Recv(message.data(), n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(message.data(), n, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    // Выводим время, затраченное на передачу
    if (rank == 0) {
        cout << "Time taken for " << n << " bytes message: " << end_time - start_time << " seconds" << endl;
    }
}

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        cerr << "This program requires exactly 2 processes." << endl;
        MPI_Finalize();
        return 1;
    }

    // Выбираем размер сообщения для теста
    int message_sizes[] = {10000, 100000, 1000000, 10000000, 100000000};  // Размеры сообщений для тестирования

    for (int n : message_sizes) {
        measure_communication(rank, size, n);\
    }

    MPI_Finalize();
    return 0;
}
