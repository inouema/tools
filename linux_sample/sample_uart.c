#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define SERIAL_PORT "/dev/ttyS0"  /* シリアルインターフェースに対応するデバイスファイル */

int main(int argc, char *argv[])
{
    char buf[255];                    /* バッファ */
    int fd;                           /* ファイルディスクリプタ */
    struct termios oldtio, newtio;    /* シリアル通信設定 */
    
    fd = open(SERIAL_PORT, O_RDWR);   /* デバイスをオープンする */
    
    ioctl(fd, TCGETS, &oldtio);       /* 現在のシリアルポートの設定を待避させる */
    newtio = oldtio;                  /* ポートの設定をコピー */
    newtio.c_cflag = ...;             /* ポートの設定をおこなう 詳細はtermios(3)参照 */
    ioctl(fd, TCSETS, &newtio);       /* ポートの設定を有効にする */
    
    read(fd, buf, sizeof(buf));       /* デバイスから255バイト読み込み */
    write(fd, buf, sizeof(buf));      /* デバイスへ255バイト書き込み */
    
    ioctl(fd, TCSETS, &oldtio);       /* ポートの設定を元に戻す */
    close(fd);                        /* デバイスのクローズ */

    return 0;
}
