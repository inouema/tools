#include <stdio.h>
#include <stdint.h>
#include "./reverse_bit.h"

int main()
{
	uint8_t var = 0x80;	
	printf("%x,%x\n", var, reverse_bit8(var));

	return 0;
}
