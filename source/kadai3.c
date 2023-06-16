#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define DELTA_X 0.02
#define D_NUM 10
#define DELTA_T 250

double a[D_NUM-1], b[D_NUM-1], c[D_NUM-1], d[D_NUM+1];

void init(double r){
  d[0] = 1000;
  d[D_NUM] = 0;
  for(int i=0;i<D_NUM;i++){
    a[i] = -r;
    b[i] = 1 + 2 * r;
    c[i] = -r;
  }
  d[1] = r * d[0] + d[1];
  d[D_NUM-1] = r * d[D_NUM] + d[D_NUM-1];
}

void thomas(int n, double a[], double b[], double c[], double d[]){
  for(int i=1;i<n;i++){
    double e = a[i] / b[i-1];
    b[i] = b[i] - e * c[i-1];
    d[i+1] = d[i+1] - e * d[i];
  }
  d[n] = d[n] / b[n-1];
  for(int i=n-1;i>0;i--){
    d[i] = (d[i] - c[i-1] * d[i+1]) / b[i-1];
  }
}

int main(){
  double aa = 1.26 / (1600 * 1050);
  double r = (aa * DELTA_T) / (DELTA_X * DELTA_X);
  for(int i=1;i<D_NUM-1;i++){
    d[i] = 0;
  }
  printf("t=%4d[s]: ", 0);
    for(int i=0;i<11;i++){
    printf("%7.1f ", d[i]);
  }
  printf("\n");

  for(int i=1;i<=20;i++){
    init(r);

    thomas(D_NUM-1, a, b, c, d);
    if(i % 4 == 0) {
      printf("t=%4d[s]: ", i * 250);
        for(int i=0;i<11;i++){
        printf("%7.1f ", d[i]);
      }
      printf("\n");
    }
  }

  return 0;
}
