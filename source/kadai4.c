#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define D_NUM 100
#define LAMBDA_S 2.22
#define CP_S 2067
#define RHO_S 917
#define ALPHA_S LAMBDA_S / (RHO_S * CP_S)
#define L 334880
#define T_A -5
#define T_M 0
#define A 0.005
#define B 0.025

double dt[D_NUM], old_dt[D_NUM];
double t_s[D_NUM][D_NUM] = {T_M};

const double dr = (B - A) / D_NUM; // Δrの算出

void init(int n, double a[], double b[], double c[], double d[]){
  double r_s = (ALPHA_S * dt[n]) / (dr * dr);

  for(int i=0;i<n;i++){
    a[i] = -r_s * (1 - 1 / (2 * (A / dr + i + 1)));
    b[i] = 1 + 2 * r_s;
    c[i] = -r_s * (1 + 1 / (2 * (A / dr + i + 1)));
    d[i] = t_s[i+1][n];
  }
  d[0] = t_s[1][n] + r_s * (1 - 1 / (2 * (A / dr + 1))) * T_A;
  d[n-1] = 0;
}

void thomas(int n, double a[], double b[], double c[], double d[]){ // 資料通りaは1-indexedとするのでa[0]は使わない
  for(int i=1;i<n;i++){
    double e = a[i] / b[i-1];
    b[i] = b[i] - e * c[i-1];
    d[i] = d[i] - e * d[i-1];
  }
  d[n-1] = d[n-1] / b[n-1];
  for(int i=n-2;i>=0;i--){
    d[i] = (d[i] - c[i] * d[i+1]) / b[i];
  }

  for(int i=1;i<=n;i++){
    t_s[i][n+1] = d[i-1];
  }
}

int is_convergence(double old_val, double new_val) {
  // 収束判定関数
  double e_temp = fabs((new_val - old_val) / old_val);
  if(e_temp > 0.00001){
    return 1;
  } else {
    return 0;
  }
}

int main(){
  double t_sum = 0;
  for(int i=0;i<D_NUM;i++) {
    dt[i] = 0;
    old_dt[i] = 0;
  }

  // Δt_0の計算
  t_s[0][1] = T_A;
  dt[0] = ((RHO_S * L) / LAMBDA_S) * ((dr * dr) / (T_M - T_A));
  printf("Δt_0  = %7.3lf[s]\n", dt[0]);
  t_sum += dt[0];

  // Δt_1の計算
  dt[1] = dt[0];
  do {
    old_dt[1] = dt[1];

    double r_s = (ALPHA_S * dt[1]) / (dr * dr);
    double q = 1 / (2 * (A / dr + 1)); // t_s1計算の簡略化変数
    t_s[1][2] = 1 / (1 + 2 * r_s) * ((1 + r_s * (1 + q)) * T_M + r_s * T_A * (1 - q));
    dt[1] = ((RHO_S * L) / LAMBDA_S) * ((dr * dr) / (T_M - t_s[1][2]));
  } while(is_convergence(old_dt[1], dt[1]));
  printf("Δt_1  = %7.3lf[s]\n", dt[1]);
  t_sum += dt[1];

  // Δt_2〜の計算
  for(int i=2;i<D_NUM;i++){
    dt[i] = dt[i-1];
    double a[i], b[i], c[i], d[i];

    do {
      old_dt[i] = dt[i];

      init(i, a, b, c, d);
      thomas(i, a, b, c, d);
      // printf("t_s[i]: %f\n", t_s[i]);
      dt[i] = ((RHO_S * L) / LAMBDA_S) * ((dr * dr) / (T_M - t_s[i][i+1]));
    } while(is_convergence(old_dt[i], dt[i]));

    printf("Δt_%-2d = %7.3lf[s]\n", i, dt[i]);
    t_sum += dt[i];
  }

  printf("\nt = %7.3lf[s]\n", t_sum);
  return 0;
}
