#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>
#include <ctime>

void measure_communication_non_blocking(int rank, int size, int n) {
    std::vector<char> message(n, 0);  // Сообщение размером n байт
    double start_time, end_time;

    // Замер времени передачи сообщения
    if (rank == 0) {
        // Процесс 0 отправляет сообщение, а процесс 1 принимает
        MPI_Request send_request, recv_request;

        start_time = MPI_Wtime();

        // Неблокирующая отправка и прием
        MPI_Isend(message.data(), n, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &send_request);
        MPI_Irecv(message.data(), n, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &recv_request);

        // Ожидаем завершения операций
        MPI_Wait(&send_request, MPI_STATUS_IGNORE);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);

        end_time = MPI_Wtime();
    } else if (rank == 1) {
        // Процесс 1 принимает сообщение, а затем отправляет его обратно
        MPI_Request send_request, recv_request;

        // Неблокирующая отправка и прием
        MPI_Irecv(message.data(), n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &recv_request);
        MPI_Isend(message.data(), n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &send_request);

        // Ожидаем завершения операций
        MPI_Wait(&send_request, MPI_STATUS_IGNORE);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
    }

    // Выводим время, затраченное на передачу
    if (rank == 0) {
        std::cout << "Time taken for " << n << " bytes message: " << end_time - start_time << " seconds" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        std::cerr << "This program requires exactly 2 processes." << std::endl;
        MPI_Finalize();
        return 1;
    }

    // Выбираем размер сообщения для теста
    int message_sizes[] = {10000, 100000, 1000000, 10000000, 100000000};  // Размеры сообщений для тестирования

    for (int n : message_sizes) {
        measure_communication_non_blocking(rank, size, n);
    }

    MPI_Finalize();
    return 0;
}
