
/*
 * Big Endian と Little Endianの判別コード
 * http://www.math.kobe-u.ac.jp/~kodama/tips-C-endian.html
 */
 

#include <stdio.h>

int main()
{
    int x=1; // 0x00000001
    
    if (*(char*)&x)
    {
        /* little endian. memory image 01 00 00 00 */
        printf("It's a Little endian\r\n");
    }
    else
    {
        /* big endian. memory image 00 00 00 01 */
        printf("It's a Big endian\r\n");
    }
    return 0;
}
