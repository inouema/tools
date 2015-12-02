/*
 * License
 * このプログラムは坂井氏のプログラムを参考にしています.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "./linuxelf.h"

#define IPL_DATA_HEADER_SIZE             (64)
#define IPL_DATA_HEADER_DATA_OFFSET      (32) /* ヘッダの先頭からの バイト数 */
#define IPL_DATA_HEADER_CHECK_SUM_OFFSET (36) /* ヘッダの先頭からの バイト数 */
#define IPL_DATA_START                   (IPL_DATA_HEADER_SIZE)

#define dprintf(fmt,...) printf("%s(): "fmt, __func__, ## __VA_ARGS__)

#define ARRAY_SIZE(a) (sizeof(a)/(sizeof(a[0])))

#define swap(a,b) ((a)^=(b)^=(a)^=(b))


/* バイナリファイルのヘッダに check sum を書き込む */
static int write_bin_check_sum(FILE *fp, const int16_t check_sum)
{
	
	fseek(fp, IPL_DATA_HEADER_CHECK_SUM_OFFSET, SEEK_SET);
	fwrite(&check_sum, sizeof(uint16_t), 1, fp);
	
	return 0;
}

/* バイナリファイルのヘッダに データ部のサイズ を書き込む */
static int write_bin_data_size(FILE *fp, const int32_t size)
{

	fseek(fp, IPL_DATA_HEADER_DATA_OFFSET, SEEK_SET);
	fwrite(&size, sizeof(uint32_t), 1, fp);
	
	return 0;
}


static uint8_t reverse_bit8(uint8_t x)
{
    x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
    x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
    return (x << 4) | (x >> 4);
}

static uint16_t reverse_bit16(uint16_t x)
{
    x = ((x & 0x5555) << 1) | ((x & 0xAAAA) >> 1);
    x = ((x & 0x3333) << 2) | ((x & 0xCCCC) >> 2);
	x = ((x & 0x0F0F) << 4) | ((x & 0xF0F0) >> 4);
    return (x << 8) | (x >> 8);
}

static uint32_t reverse_bit32(uint32_t x)
{
    x = ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
    x = ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
	x = ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
	x = ((x & 0x00FF00FF) << 8) | ((x & 0xFF00FF00) >> 8);
    return (x << 16) | (x >> 16);
}


/* FPGAアップデート用の バイナリファイルを生成する */
static int rpd2bin(FILE *fp_rpd, FILE *fp_bin)
{
	int i=0;
	int j=0;
	const uint8_t dummy[1] = {0};
	uint8_t rpd_data[4] = {0,0,0,0};
	uint8_t rpd_swap_byte[4] = {0,0,0,0};
	      
	int bin_file_size = 0;
	
	uint32_t check_sum_32;
	uint16_t check_sum_16;
	
	uint8_t read_data;
	uint32_t bin_file_offset;
	
	int rpd_fread_word_step;
	int rpd_size_byte = 0;

	int word_remain = 0;

	/*
	 * 最初に作成する 空ファイル のサイズを算出する.
	 *
	 * 算出式
	 * 1. FPGAのRPDファイルのサイズ
	 * 2. 64バイトのヘッダー部
	 * 
	 */
	fseek(fp_rpd, 0, SEEK_END);
	bin_file_size  = ftell(fp_rpd);
	bin_file_size += IPL_DATA_HEADER_SIZE;


	/*
	 * 空ファイル(bin)を作成する
	 */
	fseek(fp_bin, 0, SEEK_SET);
	
	for (i = 0; i < bin_file_size; i++) {
		fwrite(dummy, 1, 1, fp_bin);
	}
	printf("bin_file_size:%d byte(dec)\n", bin_file_size);


	/* RPDファイルのサイズを取得する */
	fseek(fp_rpd, 0, SEEK_END);
	rpd_size_byte = ftell(fp_rpd);
	printf("rpd_file_size:%d byte(dec)\n", rpd_size_byte);

	/*  FPGAデータで 4バイトきっちりじゃないことも あるかチェックする */
	/* そもそもあるのか?? */
	word_remain = rpd_size_byte % 4;
	
	
	fseek(fp_rpd, 0,                    SEEK_SET);
	fseek(fp_bin, IPL_DATA_HEADER_SIZE, SEEK_SET);
	
	/* 4バイトごと読み進めるので /4 しておく */
	rpd_fread_word_step = rpd_size_byte / 4;
	
	for (i=0; i<rpd_fread_word_step; i++) {
		
		if (i==8704)
			dprintf("i=%d\n", i);

		/* RPDファイルから 4バイト読み出し */
		fread(rpd_data, sizeof(uint8_t), ARRAY_SIZE(rpd_data), fp_rpd);
	
		if (i==8704)	
			dprintf("rpd       0x%02x 0x%02x 0x%02x 0x%02x\n", rpd_data[0],rpd_data[1],rpd_data[2],rpd_data[3]);

		/* 1バイト毎に MSB/LSB変換(bit swap) */
		for (j=0; j<ARRAY_SIZE(rpd_data); j++) {
			rpd_data[j] = reverse_bit8(rpd_data[j]);
		}

		if (i==8704)	
			dprintf("swap bit  0x%02x 0x%02x 0x%02x 0x%02x\n", rpd_data[0],rpd_data[1],rpd_data[2],rpd_data[3]);

		/* do, byte swap. */
		//swap(rpd_data[0], rpd_data[3]);
		//swap(rpd_data[1], rpd_data[2]);
		rpd_swap_byte[3] = rpd_data[0];
		rpd_swap_byte[2] = rpd_data[1];
		rpd_swap_byte[1] = rpd_data[2];
		rpd_swap_byte[0] = rpd_data[3];


		if (i==8704)	
			dprintf("swap byte 0x%02x 0x%02x 0x%02x 0x%02x\n", rpd_swap_byte[0],rpd_swap_byte[1],rpd_swap_byte[2],rpd_swap_byte[3]);

		/* 4バイトを書き込む */
		//fwrite(&rpd_data, sizeof(uint8_t), ARRAY_SIZE(rpd_data), fp_bin);
		fwrite(rpd_swap_byte, sizeof(uint8_t), ARRAY_SIZE(rpd_swap_byte), fp_bin);
		
	}
	
	/*
	 * 16bit check sum 算出
	 */
	fseek(fp_bin, IPL_DATA_START, SEEK_SET);
	
	check_sum_16 = 0;
	check_sum_32 = 0;
	
	/* IPL_DATA_HEADER(64バイト) を除く領域(実際のデータ)だけが check_sum の対象 */
	for (i=0; i < (bin_file_size - IPL_DATA_START); i++) {
		
		fread(&read_data, 1, 1, fp_bin);
		
		check_sum_32 += read_data;
		//dprintf("check_sum_16:0x%04x\n", check_sum_16);
	}
	
	/* データ部のチェックサムを書き込む */
	check_sum_16 = (check_sum_32 & 0x0000FFFF);
	write_bin_check_sum(fp_bin, check_sum_16);
	
	dprintf("\n");
	dprintf("data_block's check_sum_16:0x%04x\n", check_sum_16);

	/* データ部のサイズを書き込む */
	write_bin_data_size(fp_bin, bin_file_size - IPL_DATA_HEADER_SIZE);
	
	dprintf("\n");
	dprintf("bin_file_size: %d(dec) 0x%08x(hex)\n", bin_file_size, bin_file_size);
	dprintf("data_size    : %d(dec) 0x%08x(hex)\n", (bin_file_size - IPL_DATA_HEADER_SIZE), (bin_file_size - IPL_DATA_HEADER_SIZE));

	printf("\n");
	return (0);
	
}



/*
 * strtol関数 wrap
 * 引数：
 * str:	変換元の文字列。strtol関数の第１引数と同じ。
 * radix:  基数. strtol関数の第３引数と同じ.
 * result: 変換結果を受け取るアドレス. NULL は不可。変換に失敗した場合には、何も格納されない.
 * 戻り値: 変換が成功したら 0以外、失敗したら 0 が返される。
 */
int my_strtol(const char* str, int radix, long int* result)
{
	long int num;
	char* end;

	assert( str != NULL );
	assert( result != NULL );

	errno = 0;
	num = strtol( str, &end, radix );
	if( errno == ERANGE && (num == LONG_MAX || num == LONG_MIN) ){
		return 0;    /* 変換結果が表現できない */
	}
	else if( str == end ){
		return 0;    /* １文字も変換できない */
	}

	*result = num;
	
	return 1;
}


/*
 * 第1引数: input  RPD file name.(ex. ts500_fpga.rpd)
 * 第2引数: output RPD file name.(ex. ts500_fpga.bin)
 */
int main(int argc, char *argv[])
{
	int c;
	int fd;
	struct stat sb;
	char *head;

	FILE    *fp_rpd;
	FILE    *fp_bin;

	fp_rpd = fopen(argv[1], "rb");
	if (NULL == fp_rpd) {
		exit(2);
	}
	
	/* 一度, 新規作成モード(wb)で開くことで, 新規ファイルとしている */
	if ((fp_bin = fopen(argv[2], "wb")) == NULL)  { exit(2); }
	fclose(fp_bin);
	if ((fp_bin = fopen(argv[2], "r+b")) == NULL) { exit(2); }

	rpd2bin(fp_rpd, fp_bin);
	
	close(fp_rpd);
	close(fp_bin);

	exit(0);
}
