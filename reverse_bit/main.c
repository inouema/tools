#include <stdio.h>
#include <stdint.h>
#include "./reverse_bit.h"

int main()
{
	uint8_t  var8  = 0x80;
	uint16_t var16 = 0x8080;
	uint32_t var32 = 0x80808080;
	printf("%x,%x\n", var8, reverse_bit8(var8));
	printf("%x,%x\n", var16, reverse_bit16(var16));
	printf("%x,%x\n", var32, reverse_bit32(var32));

	return 0;
}
