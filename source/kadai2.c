#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define DELTA_X 0.02
#define DIVISION_NUMBER 10
#define DELTA_T 250

int main(){
  float temp_field[21][DIVISION_NUMBER + 1];
  memset(temp_field, -1, sizeof(temp_field));

  // 初期条件・境界条件設定
  for(int i=0;i<DIVISION_NUMBER;i++){
    temp_field[0][i] = 0;
  }
  for(int t=0;t<21;t++){
    temp_field[t][0] = 1000;
    temp_field[t][DIVISION_NUMBER] = 0;
  }

  // 計算
  float a = 1.26 / (1600 * 1050);
  float r = (a * DELTA_T) / (DELTA_X * DELTA_X);

  for(int t=0;t<21;t++){
    for(int i=1;i<DIVISION_NUMBER;i++){
      temp_field[t+1][i] = (1 - (2 * r)) * temp_field[t][i] + r * (temp_field[t][i-1] + temp_field[t][i+1]);
    }
  }

  // 結果出力
  for(int t=0;t<=5;t++){
    if(t == 0) { printf("t=   %d[s]: ", 0); }
    else { printf("t=%d[s]: ", t * 4 * DELTA_T); }
    for(int i=0;i<DIVISION_NUMBER;i++){
      printf("%7.1f ", temp_field[t*4][i]);
    }
    printf("\n");
  }

  return 0;
}
