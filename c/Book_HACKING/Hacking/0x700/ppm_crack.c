/*********************************************************\
*  Password Probability Matrix   *    File: ppm_crack.c   *
***********************************************************
*                                                         *
*  Author:        Jon Erickson <matrix@phiral.com>        *
*  Organization:  Phiral Research Laboratories            *
*                                                         *
*  This is the crack program for the PPM proof of concept.*
*  It uses an existing file called 4char.ppm, which       *
*  contains information regarding all possible 4-         *
*  character passwords salted with 'je'.  This file can   *
*  be generated with the corresponding ppm_gen.c program. *
*                                                         *
\*********************************************************/

#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 16384
#define WIDTH  1129
#define DEPTH 8
#define SIZE HEIGHT * WIDTH * DEPTH
#define DCM HEIGHT * WIDTH

/* �P��̃n�b�V���o�C�g��񋓒l�Ƀ}�b�v����B */
int enum_hashbyte(char a) {
   int i, j;
   i = (int)a;
   if((i >= 46) && (i <= 57))
      j = i - 46;
   else if ((i >= 65) && (i <= 90))
      j = i - 53;
   else if ((i >= 97) && (i <= 122))
      j = i - 59;
   return j;
}

/* 3�̃n�b�V���o�C�g��񋓒l�Ƀ}�b�v����B */
int enum_hashtriplet(char a, char b, char c) {
   return (((enum_hashbyte(c)%4)*4096)+(enum_hashbyte(a)*64)+enum_hashbyte(b));
}

/* 2�̃x�N�^����������B */
void merge(char *vector1, char *vector2) {
   int i;
   for(i=0; i < WIDTH; i++)
      vector1[i] &= vector2[i];
}

/* vector���ŁA�����n���ꂽindex�ʒu�ɂ���r�b�g��Ԃ��B */
int get_vector_bit(char *vector, int index) {
   return ((vector[(index/8)]&(1<<(index%8)))>>(index%8));
}

/* �����n���ꂽ�x�N�^���ɂ����镽���̃y�A�̐��𐔂���B */
int count_vector_bits(char *vector) {
   int i, count=0;
   for(i=0; i < 9025; i++)
      count += get_vector_bit(vector, i);
   return count;
}

/* �x�N�^�񋓓��ŃI���ɂȂ��Ă���r�b�g�ɓ����镽���̃y�A���o�͂���B */
void print_vector(char *vector) {
   int i, a, b, val;
   for(i=0; i < 9025; i++) {
      if(get_vector_bit(vector, i) == 1) { // �r�b�g���I���ł���ꍇ�A
         a = i / 95;                  // �����̃y�A��
         b = i - (a * 95);            // �Z�o���A
         printf("%c%c ",a+32, b+32);  // �o�͂���B
      }
   }
   printf("\n");
}

/* ���b�Z�[�W���o�͂��ďI������B */
void barf(char *message, char *extra) {
   printf(message, extra);
   exit(1);
}

/* �������ꂽ4char.ppm�t�@�C�����g�p����4�����̃p�X���[�h����ǂ���B */
int main(int argc, char *argv[]) {
  char *pass, plain[5];
  unsigned char bin_vector1[WIDTH], bin_vector2[WIDTH], temp_vector[WIDTH];
  char prob_vector1[2][9025];
  char prob_vector2[2][9025];
  int a, b, i, j, len, pv1_len=0, pv2_len=0;
  FILE *fd;

  if(argc < 1)
     barf("�g�p���@�F %s ���p�X���[�h�n�b�V�����i4char.ppm�t�@�C�����g�p�j\n", argv[0]);

  if(!(fd = fopen("4char.ppm", "r")))
     barf("�G���[�F PPM �t�@�C����ǂݍ��݃��[�h�ŃI�[�v���ł��܂���ł����B\n", NULL);

  pass = argv[1]; // �ŏ��̈����̓p�X���[�h�n�b�V��

  printf("�ŏ���2�����ɑ΂��čl�����镽���̃o�C�g�̃t�B���^�����O�F\n");

  fseek(fd,(DCM*0)+enum_hashtriplet(pass[2], pass[3], pass[4])*WIDTH, SEEK_SET);
  fread(bin_vector1, WIDTH, 1, fd); // �n�b�V����2-4�o�C�g�ڂɊ֘A�t����ꂽ�x�N�^��ǂݍ��ށB

  len = count_vector_bits(bin_vector1);
  printf("4�̃x�N�^�̂���1�̂݁F\t%d �̕����̃y�A�i��L���� %0.2f%% �j\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*1)+enum_hashtriplet(pass[4], pass[5], pass[6])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // �n�b�V����4-6�o�C�g�ڂɊ֘A�t����ꂽ�x�N�^��ǂݍ��ށB
  merge(bin_vector1, temp_vector);  // �ŏ��̃x�N�^�Ɍ�������B

  len = count_vector_bits(bin_vector1);
  printf("�x�N�^1��2�͌����ς݁F\t%d �̕����̃y�A�i��L���� %0.2f%% �j\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*2)+enum_hashtriplet(pass[6], pass[7], pass[8])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // �n�b�V����6-8�o�C�g�ڂɊ֘A�t����ꂽ�x�N�^��ǂݍ��ށB
  merge(bin_vector1, temp_vector);  // �ŏ���2�̃x�N�^�Ɍ�������B

  len = count_vector_bits(bin_vector1);
  printf("�ŏ���3�̃x�N�^�͌����ς݁F\t%d �̕����̃y�A�i��L���� %0.2f%% �j\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*3)+enum_hashtriplet(pass[8], pass[9],pass[10])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // �n�b�V����8-10�o�C�g�ڂɊ֘A�t����ꂽ�x�N�^��ǂݍ��ށB
  merge(bin_vector1, temp_vector);  // ���̃x�N�^�Ɍ�������B

  len = count_vector_bits(bin_vector1);
  printf("4�̃x�N�^���ׂĂ͌����ς݁F\t%d �̕����̃y�A�i��L���� %0.2f%% �j\n", len, len*100.0/9025.0);

  printf("�ŏ���2�o�C�g�ɑ΂��čl�����镽���̃y�A�F\n");
  print_vector(bin_vector1);

  printf("\n�Ō��2�����ɑ΂��ĉ\\�ȕ����̃t�B���^�����O�F\n");

  fseek(fd,(DCM*4)+enum_hashtriplet(pass[2], pass[3], pass[4])*WIDTH, SEEK_SET);
  fread(bin_vector2, WIDTH, 1, fd); // �n�b�V����2-4�o�C�g�ڂɊ֘A�t����ꂽ�x�N�^��ǂݍ��ށB

  len = count_vector_bits(bin_vector2);
  printf("4�̃x�N�^�̂���1�̂݁F:\t%d �̕����̃y�A�i��L���� %0.2f%% �j\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*5)+enum_hashtriplet(pass[4], pass[5], pass[6])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // �n�b�V����4-6�o�C�g�ڂɊ֘A�t����ꂽ�x�N�^��ǂݍ��ށB
  merge(bin_vector2, temp_vector);  // �ŏ��̃x�N�^�Ɍ�������B

  len = count_vector_bits(bin_vector2);
  printf("�x�N�^1��2�͌����ς݁F\t%d �̕����̃y�A�i��L���� %0.2f%% �j\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*6)+enum_hashtriplet(pass[6], pass[7], pass[8])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // �n�b�V����6-8�o�C�g�ڂɊ֘A�t����ꂽ�x�N�^��ǂݍ��ށB
  merge(bin_vector2, temp_vector);  // �ŏ���2�̃x�N�^�Ɍ�������B

  len = count_vector_bits(bin_vector2);
  printf("�ŏ���3�̃x�N�^�͌����ς݁F\t%d �̕����̃y�A�i��L���� %0.2f%% �j\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*7)+enum_hashtriplet(pass[8], pass[9],pass[10])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // �n�b�V����8-10�o�C�g�ڂɊ֘A�t����ꂽ�x�N�^��ǂݍ��ށB
  merge(bin_vector2, temp_vector);  // ���̑��̃x�N�^�Ɍ�������B

  len = count_vector_bits(bin_vector2);
  printf("4�̃x�N�^���ׂĂ͌����ς݁F\t%d �̕����̃y�A�i��L���� %0.2f%% �j\n", len, len*100.0/9025.0);

  printf("�Ō��2�o�C�g�ɂ��čl�����镽���̃y�A�F\n");
  print_vector(bin_vector2);

  printf("�m���x�N�^���쐬���ł�...\n");
  for(i=0; i < 9025; i++) { // �l������ŏ���2�̕����o�C�g����������B
    if(get_vector_bit(bin_vector1, i)==1) {;
      prob_vector1[0][pv1_len] = i / 95;
      prob_vector1[1][pv1_len] = i - (prob_vector1[0][pv1_len] * 95);
      pv1_len++;
    }
  }
  for(i=0; i < 9025; i++) { // �l������Ō��2�̕����o�C�g����������B
    if(get_vector_bit(bin_vector2, i)) {
      prob_vector2[0][pv2_len] = i / 95;
      prob_vector2[1][pv2_len] = i - (prob_vector2[0][pv2_len] * 95);
      pv2_len++;
    }
  }

  printf("�c�� %d �̉\\������ǂ��Ă��܂�...\n", pv1_len*pv2_len);
  for(i=0; i < pv1_len; i++) {
    for(j=0; j < pv2_len; j++) {
      plain[0] = prob_vector1[0][i] + 32;
      plain[1] = prob_vector1[1][i] + 32;
      plain[2] = prob_vector2[0][j] + 32;
      plain[3] = prob_vector2[1][j] + 32;
      plain[4] = 0;
      if(strcmp(crypt(plain, "je"), pass) == 0) {
        printf("�p�X���[�h�F  %s\n", plain);
        i = 31337;
        j = 31337;
      }
    }
  }
  if(i < 31337)
    printf("�p�X���[�h�� salt �� 'je' �łȂ����A������ 4 ���ł͂���܂���B\n");

  fclose(fd);
}
