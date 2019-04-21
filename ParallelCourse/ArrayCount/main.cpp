#include <math.h>
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    size_t N = 10;
    if (argc != 1) {
        N = (size_t) strtol(argv[1],NULL,10);
    }

    int rank, P;

    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;

    size_t bunch = N/(P-1);
    size_t remainder = N%(P-1);

    size_t size = bunch;

    if (rank == 0) {
        size = N;
    } else if (rank == P-1){
        bunch += remainder;
        size += remainder;
    }

    int* array = new int[size];

    int sum = 0;
    int answer = 0;

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = i;
        }

        int pos = 0;
        for (int i = 1; i < P-1; i++) {
            MPI_Send(&array[pos], bunch, MPI_INT, i, i, MPI_COMM_WORLD);
            pos += bunch;
        }

        MPI_Send(&array[pos], bunch + remainder, MPI_INT, P-1, P-1, MPI_COMM_WORLD);

        MPI_Reduce(&sum, &answer, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        std::cout << "S = " << answer << "\n";
        std::cout << "S0 = " << (N-1)*N/2 << "\n";
    } else {
        MPI_Recv(&array[0], size, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
        for(int i = 0; i < size; i++) {
            sum += array[i];
        }
        std::cout << "rank: " << rank << ", sum: " << sum << "\n";
        MPI_Reduce(&sum, &answer, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    delete [] array;
    MPI_Finalize();

    return 0;
}

