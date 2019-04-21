#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <cmath>
#include <vector>


int main(int argc, char* argv[]) {
  double start, end;
  int threads = 1;

  if (argc != 1) {
    threads = (int) strtol(argv[1],NULL,10);
  }

  size_t N = 10;
  std::vector<std::vector<double> > A (N, std::vector<double>(N, 1));
  for (int i = 0; i < N; i++) {
    A[i][i] = N + i + 1;
  }
  std::vector<double> b(N);
  for (int i = 0; i < N; i++) {
    b[i] = 2*N - i;
  }
  std::vector<double> x(N);
  std::vector<double> last_x(N);
  double epsilon = 0.000001;
  int max_iterations = 1000000;
  int iter = 0;
  bool stop = false;

  start = omp_get_wtime();
  #pragma omp parallel shared(x, last_x, stop)
  {
    while (!stop) {
      #pragma omp for
      for (int i = 0; i < N; i++) {
        double diff_x_i = 0.0;
        for (int j = 0; j < N; j++) {
          if (i != j) {
            diff_x_i += A[i][j] * x[j];
          }
        }
        double x_new = (b[i] - diff_x_i) / A[i][i];
        #pragma omp critical
        {
          x[i] = x_new;
        }
      }
      stop = true;
      #pragma omp for
      for (int i = 0; i < N; i++) {
        if (std::abs(last_x[i] - x[i]) > epsilon) {
          stop = false;
        }
        #pragma omp critical
        {
          last_x[i] = x[i];
        }
      }
      iter += 1;
    }
  }

  for (int i = 0; i < N; i++) {
    std::cout << x[i] << "\n";
  }

  end = omp_get_wtime();
  std::cout << "iterations: " <<  iter << std::endl;
  std::cout << "time: " <<  end - start << std::endl;
  return 0;
}
