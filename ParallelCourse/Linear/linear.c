#include <iostream>
#include <time.h>

using namespace std;

#define nl "\n"

void LinearSolve(double **& M, double *& V, const int N, bool parallel, int threads){
   //...
   for (int i=0;i<N;i++){
      #pragma omp parallel for num_threads(threads) if(parallel)
      for (int j=i+1;j<N;j++){
         double aux, * Mi=M[i], * Mj=M[j];
         aux=Mj[i]/Mi[i];
         Mj[i]=0;
         for (int k=i+1;k<N;k++) {
            Mj[k]-=Mi[k]*aux;
         };
         V[j]-=V[i]*aux;
      };
   };
   //...
};

class Time {
   clock_t startC, endC;
   time_t startT, endT;
   public:
      void start() {startC=clock(); time (&startT);};
      void end() {endC=clock();  time (&endT);};
      double timedifCPU() {return(double(endC-startC)/CLOCKS_PER_SEC);};
      int timedif() {return(int(difftime (endT,startT)));};
};

int main (){
  Time t;
  double ** M, * V;
  int N=1000;
  cout<<"number of equations "<<N<<nl<<nl;

  M= new double * [N];
  V=new double [N];
  for (int i=0;i<N;i++){
     M[i]=new double [N];
  };

  for (int m=1;m<=16;m=2*m){
     cout<<m<<" threads"<<nl;

     for (int i=0;i<N;i++){
        V[i]=i+1.5*i*i;
        for (int j=0;j<N;j++){
           M[i][j]=(j+2.3)/(i-0.2)+(i+2)/(j+3); //some function to get regular matrix
        };
     };

     t.start();
     LinearSolve(M,V,N,m!=1,m);
     t.end();

     cout<<"time "<<t.timedif()<<", CPU time "<<t.timedifCPU()<<nl<<nl;
  };
}
