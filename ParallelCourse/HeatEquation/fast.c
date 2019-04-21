#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

const char *filename = "./temp.txt";
void SaveToFile(float* final, int height, int width) {
    FILE *f = fopen(filename, "a");
    for (int i = 1; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(f, "%8.1f", final[i*width + j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(int argc, char *argv[]) {
    clock_t start, end;
    start = clock();

    MPI_Init(&argc, &argv);

    const float u_0 = 5, u_l = 80, u_r = 30;
    const float tau = 0.005;
    const float length = 0.5;

    int rank, P;
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int height =  length/tau / P + 2;
    if (rank <  (int)(length/tau) % P) {
        height += 1;
    }
    int width = length/tau + 2;

    float* middle = (float*) malloc(height * width * sizeof(float));
    float* final = (float *) malloc(height * width * sizeof(float));

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            final[i*width + j] = u_0;
        }
        middle[i * width] = u_l;
        middle[i * width + width - 1] = u_r;
        final[i*width] = u_l;
        final[i*width + width - 1] = u_r;
    }

    const int iterations = (int) 60 / 0.002;
    const float k = 401.0/(8960.0*380.0) / (tau*tau);
    for (int t = 0; t < iterations; t++) {
        MPI_Request Request;
        if (rank != 0) {
            MPI_Isend(&final[width], width, MPI_FLOAT, rank - 1, rank - 1, MPI_COMM_WORLD, &Request);
        }
        if (rank != P - 1) {
            MPI_Isend(&final[(height - 2)*width], width, MPI_FLOAT, rank + 1, rank + 1, MPI_COMM_WORLD, &Request);
        }

        MPI_Status Status;
        if (rank != 0) {
            MPI_Recv(&final[0], width, MPI_FLOAT, rank - 1, rank, MPI_COMM_WORLD, &Status);
        }
        if (rank != P - 1) {
            MPI_Recv(&final[(height - 1)*width], width, MPI_FLOAT, rank + 1, rank, MPI_COMM_WORLD, &Status);
        }

        for (int i = 1; i < height - 1; i++) {
          for (int j = 1; j < width - 1; j++) {
              middle[i * width + j] = final[i*width + j] + k * tau * (final[(i + 1)*width + j] - 2 * final[i*width + j] + final[(i - 1)*width + j]);
          }
        }
        for (int i = 1; i < height - 1; i++) {
            for (int j = 1; j < width - 1; j++) {
                final[i*width + j] = middle[i*width + j] + k * tau * (middle[i * width + j + 1] - 2 * middle[i * width + j] + middle[i * width + j - 1]);
            }
        }
    }

    if (rank != 0) {
        MPI_Request Request;
        MPI_Isend(&final[0], height * width, MPI_FLOAT, 0, rank, MPI_COMM_WORLD, &Request);
    }

    if (rank == 0) {
        remove(filename);
        SaveToFile(final, height-2, width);
        for (int i = 1; i < P; ++i) {
            MPI_Status Status;
            MPI_Recv(&final[0], height * width, MPI_FLOAT, i, i, MPI_COMM_WORLD, &Status);
            SaveToFile(final, height-2, width);
        }
        end = clock();
        printf("took %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);
    }

    free(final);
    free(middle);

    MPI_Finalize();

    return 0;
}
