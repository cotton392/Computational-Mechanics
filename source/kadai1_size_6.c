#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define INITALIZE_NUMBER 25 // 初期値の定義

int is_convergence(float temp_field[7][7][2]) {
  // 収束判定関数
  for(int y=0;y<7;y++) {
    for(int x=0;x<7;x++) {
      if(temp_field[y][x][1] == 0) continue;
      float e_temp = fabs((temp_field[y][x][0] - temp_field[y][x][1]) / temp_field[y][x][1]);

      if(e_temp > 0.00001) return 0;
    }
  }

  return 1;
}

void temp_field_copy(float temp_field[7][7][2]) {
  // 温度分布のコピー関数
  for(int y=0;y<7;y++) {
    for(int x=0;x<7;x++) {
      temp_field[y][x][1] = temp_field[y][x][0];
    }
  }
}

void print_temp_field(float temp_field[7][7][2], float strict_temp_field[7][7], bool cond) {
  // 温度分布の出力関数
  for(int y=0;y<7;y++) {
    for(int x=0;x<7;x++) {
      if(temp_field[y][x][cond] >= 0 && temp_field[y][x][cond] < 10) {
        printf("     %.3f    ", temp_field[y][x][cond]);
      } else {
        printf("    %.3f    ", temp_field[y][x][cond]);
      }
    }
    printf("\n");
    for(int x=0;x<7;x++) {
      if(strict_temp_field[y][x] >= 0 && strict_temp_field[y][x] < 10) {
        printf("   ( %.3f )  ", strict_temp_field[y][x]);
      } else {
        printf("  ( %.3f )  ", strict_temp_field[y][x]);
      }
    }
    printf("\n");
  }
}

int main() {
  float temp_field[7][7][2]; // [y][x][0]は新しい計算結果を格納, [y][x][1]は古い計算結果を格納
  float strict_temp_field[7][7];

  // 初期状態の設定・厳密解の計算
  for(int y=0;y<7;y++){
    for(int x=0;x<7;x++) {
      if(x == 0 || y == 6 || x == 6) {
        temp_field[y][x][0] = 0; // (x, 0)と(0, y)を0に設定
        temp_field[y][x][1] = 0;
        strict_temp_field[y][x] = 0;
      } else if(y == 0) {
        temp_field[y][x][0] = 50 * sin((M_PI / 6) * x); // 両端で0になるように分割数で割る
        temp_field[y][x][1] = 50 * sin((M_PI / 6) * x);
        strict_temp_field[y][x] = 50 * sin((M_PI / 6) * x);
      } else {
        temp_field[y][x][0] = -1; // 未計算領域を-1とする
        temp_field[y][x][1] = -1;
        strict_temp_field[y][x] = (50 / sinh(M_PI)) * sin((M_PI / 6) * (6-x)) * sinh((M_PI / 6) * (6-y));
      }
    }
  }

  // 初期値の設定と初期状態の表示
  temp_field[2][1][0] = INITALIZE_NUMBER;
  temp_field[1][2][0] = INITALIZE_NUMBER;
  printf("    ====================================\n");
  printf("    Initialized temperature distribution\n");
  printf("    ====================================\n");
  print_temp_field(temp_field, strict_temp_field, 0);

  printf("\n");
  printf("    calculating...\n");
  printf("\n");

  // ガウス・ザイデル法による反復計算
  float result;
  while(1) {
    for(int y=1;y<6;y++){
      for(int x=1;x<6;x++) {
        temp_field[y][x][0] = (temp_field[y-1][x][0] + temp_field[y][x-1][0] + temp_field[y][x+1][0] + temp_field[y+1][x][0]) / 4;
      }
    }
    if(is_convergence(temp_field)) break;
    temp_field_copy(temp_field);
  }

  // 計算結果の表示
  printf("    ====================================\n");
  printf("                   result               \n");
  printf("    ====================================\n");
  print_temp_field(temp_field, strict_temp_field, 0);

  return 0;
}
