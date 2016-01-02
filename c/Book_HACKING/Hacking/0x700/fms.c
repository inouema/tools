#include <stdio.h>

/* RC4 ストリーム暗号 */
int RC4(int *IV, int *key) {
  int K[256];
  int S[256];
  int seed[16];
  int i, j, k, t;

  // 種の値は IV と鍵を連結したものである。
  for(k=0; k<3; k++)
    seed[k] = IV[k];
  for(k=0; k<13; k++)
    seed[k+3] = key[k];

  // -= Key Scheduling Algorithm (KSA) =-
  // 配列の初期化を行う。
  for(k=0; k<256; k++) {
    S[k] = k;
    K[k] = seed[k%16];
  }

  j=0;
  for(i=0; i < 256; i++) {
    j = (j + S[i] + K[i]) % 256;
    t=S[i]; S[i]=S[j]; S[j]=t;  // S[i] と S[j] の値を交換する。
  }

  // キーストリームの最初のバイトを判定するため、PRGA の 1 手順目を実行する。
  i = 0;
  j = 0;

  i = i + 1;
  j = j + S[i];

  t=S[i]; S[i]=S[j]; S[j]=t;  // S[i] と S[j] の値を交換する。

  k = (S[i] + S[j]) % 256;

  return S[k];
}

int main(int argc, char *argv[]) {
  int K[256];
  int S[256];

  int IV[3];
  int key[13] = {1, 2, 3, 4, 5, 66, 75, 123, 99, 100, 123, 43, 213};
  int seed[16];
  int N = 256;
  int i, j, k, t, x, A;
  int keystream, keybyte;

  int max_result, max_count;
  int results[256];

  int known_j, known_S;

  if(argc < 2) {
    printf("使用方法： %s ＜攻撃を行う鍵のバイト位置＞\n", argv[0]);
    exit(0);
  }
  A = atoi(argv[1]);
  if((A > 12) || (A < 0)) {
    printf("鍵のバイトには 0 から 12 を指定してください。\n");
    exit(0);
  }

  for(k=0; k < 256; k++)
    results[k] = 0;

  IV[0] = A + 3;
  IV[1] = N - 1;

  for(x=0; x < 256; x++) {
    IV[2] = x;

    keystream = RC4(IV, key);
    printf("使用 IV： (%d, %d, %d), キーストリームの最初のバイトは %u です。\n",
        IV[0], IV[1], IV[2], keystream);

    printf("KSA の最初の %d 手順を実行中です... ", A+3);

    // 種の値は IV と鍵を連結したものである。
    for(k=0; k<3; k++)
      seed[k] = IV[k];
    for(k=0; k<13; k++)
      seed[k+3] = key[k];

    // -= Key Scheduling Algorithm (KSA) =-
    // 配列の初期化を行う。
    for(k=0; k<256; k++) {
      S[k] = k;
      K[k] = seed[k%16];
    }

    j=0;
    for(i=0; i < (A + 3); i++) {
      j = (j + S[i] + K[i]) % 256;
      t = S[i];
      S[i] = S[j];
      S[j] = t;
    }

    if(j < 2) {  // j が 2 未満の場合、S[0] または S[1] が干渉しあっている。
      printf("S[0] または S[1] が干渉しあっているため、結果を破棄します...\n");
    } else {
      known_j = j;
      known_S = S[A+3];
      printf("  KSA の %d 手順目： j=%d, S[%d]=%d\n",
          A+3, known_j, A+3, known_S);
      keybyte = keystream - known_j - known_S;

      while(keybyte < 0)
        keybyte = keybyte + 256;
      printf("鍵[%d] の予想 = %d - %d - %d = %d\n",
          A, keystream, known_j, known_S, keybyte);
      results[keybyte] = results[keybyte] + 1;
    }
  }
  max_result = -1;
  max_count = 0;

  for(k=0; k < 256; k++) {
    if(max_count < results[k]) {
      max_count = results[k];
      max_result = k;
    }
  }
  printf("\n鍵[%d] の頻度テーブル（* = 最も頻度の高いもの）\n", A);
  for(k=0; k < 32; k++) {
    for(i=0; i < 8; i++) {
      t = k + i * 32;
      if(max_result == t)
        printf("%3d %2d*| ", t, results[t]);
      else
        printf("%3d %2d | ", t, results[t]);
    }
    printf("\n");
  }

  printf("\n実際の鍵 = （");
  for(k=0; k < 12; k++)
    printf("%d, ",key[k]);
  printf("%d）\n", key[12]);

  printf("鍵[%d] はおそらく %d です。\n", A, max_result);
}