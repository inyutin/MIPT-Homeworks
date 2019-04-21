#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

double func(double x) {
  return 4/(1+x*x);
}

int main(int argc, char* argv[]) {
  double begin, end;

  int threads = 1;
  if (argc != 1) {
    threads = (int) strtol(argv[1],NULL,10);
  }
  omp_set_num_threads(threads);

  double total = 0;
  int iter = pow(10,9);

  begin = omp_get_wtime();
  #pragma omp parallel shared(total)
  {
    double sum=0;
    #pragma omp for
    for(int i=0; i<iter; i++) {
      double tmp = 1.0 * i;
      double left = tmp/iter;
      double right = (tmp+1)/iter;
      sum+=(func(left)+func(right))/2;
    }
    #pragma omp critical
    {
      total+=sum;
    }
  }
  end = omp_get_wtime();
  printf("The value of the integral is %f\n", total/iter);
  printf("time: %f\n", end - begin);
  return 0;
}
