// 26.09.2016 DL9LJ: IC-735 remote control
// add this to your sdr-transceiver-hpsdr.c file


// GLOBAL
#include <termios.h>

int uart_fd = -1;
unsigned char BUF_TX[10] = {0xFE, 0xFE, 0x04, 0xE0, 0x05, 0x00, 0x00, 0x08, 0x01, 0xFD};
unsigned char band, oldband;


// alex_write()
uint32_t ficom = 0;

ficom = alex_data_1;

if(ficom < 2000000) band = 0x01;       /* 160m */
else if(ficom < 4000000) band = 0x03;  /*  80m */
else if(ficom < 8000000) band = 0x07;  /*  40m */
else if(ficom < 11000000) band = 0x10; /*  30m */
else if(ficom < 15000000) band = 0x14; /*  20m */
else if(ficom < 20000000) band = 0x18; /*  17m */
else if(ficom < 22000000) band = 0x21; /*  15m */
else if(ficom < 26000000) band = 0x24; /*  12m */
else band = 0x28;                      /*  10m */

switch(band) {
 case 0x01:
    BUF_TX[7] = 0x80;
    break;
 case 0x03:
    BUF_TX[7] = 0x50;
    break;
 default:
    BUF_TX[7] = 0x00;
}

BUF_TX[8] = band;

if(oldband != band)
{
  write(uart_fd, &BUF_TX[0], 10);
  oldband = band;
}


// main()
uart_fd = open("/dev/ttyPS1", O_RDWR | O_NOCTTY | O_NDELAY);

  if (uart_fd == -1) {
        perror("UART open");
  }

  struct termios options;
  tcgetattr(uart_fd, &options);
	options.c_cflag = B1200 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
  tcflush(uart_fd, TCIFLUSH);
  tcsetattr(uart_fd, TCSANOW, &options);

write(uart_fd, &BUF_TX[0], 10);
