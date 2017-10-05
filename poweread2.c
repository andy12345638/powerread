#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int set_interface_attribs(int fd, int speed)
{
	struct termios tty;

	if (tcgetattr(fd, &tty) < 0) {
		printf("Error from tcgetattr: %s\n", strerror(errno));
		return -1;
	}

	cfsetospeed(&tty, (speed_t)speed);
	cfsetispeed(&tty, (speed_t)speed);

	tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;         /* 8-bit characters */
	tty.c_cflag &= ~PARENB;     /* no parity bit */
	tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
	tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

	/* setup for non-canonical mode */
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tty.c_oflag &= ~OPOST;

	/* fetch bytes as they become available */
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 1;

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		printf("Error from tcsetattr: %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

float powerread (int fd,int a){
	int wlen;//回傳寫入狀態
	unsigned char buffer[8];//緩衝區
	int checksum = 0;//檢驗碼
	unsigned char   *p = buffer;//操作緩衝區的指標，用來讀取緩衝區
	int rlength = 0;//操作緩出區的位置長度，用來寫入緩衝區
	int length;//回傳讀取狀態
	float ret;//將緩衝區資料換成浮點數，最後return輸出
	switch(a){
	case 1:
		wlen = write(fd, "\xB0\xC0\xA8\x01\x01\x00\x1A", 7);//寫入讀取電壓指令，並確認是否長度正確。
		printf("sent B0C0A80101001A\n");//read v
		if (wlen != 7) {
			printf("Error from write: %d, %d\n", wlen, errno);
		}
		tcdrain(fd);    /* delay for output */
		//usleep(50000);	
		//rlength = 0;
		do//每次讀取一個char(8bit)，連續7次。同時累加shecksum作為檢查用。
		{	
			length = read(fd, buffer + rlength, 1);
			checksum += buffer[rlength];		
			rlength += length;
		} while(rlength < 7);
			checksum -= buffer[6];
		
			printf("%x,%x \n",buffer[6],checksum);
		if (buffer[0] != 0xA0 || buffer[6] != (checksum & 0xFF)){
			printf("checksum error! \n");
			break;		
		}
		buffer[rlength] = '\0';
		ret = (float)p[2]+(float)p[3]/10.0;//v
		printf("V=%4.1f \n",ret);
		
		break;
	case 2:
		wlen = write(fd, "\xB1\xC0\xA8\x01\x01\x00\x1B", 7);
		printf("sent B1C0A80101001B\n");//read v
		if (wlen != 7) {
			printf("Error from write: %d, %d\n", wlen, errno);
		}
		tcdrain(fd);    /* delay for output */
		//sleep(1);	
		//rlength = 0;
		do
		{	
			length = read(fd, buffer + rlength, 1);
			checksum += buffer[rlength];		
			rlength += length;
		} while(rlength < 7);
			checksum -= buffer[6];
		
			printf("%x,%x \n",buffer[6],checksum);
		if (buffer[0] != 0xA1 || buffer[6] != (checksum & 0xFF)){
			printf("checksum error! \n");
			break;		
		}
		
		buffer[rlength] = '\0';
		ret = (float)p[2]+(float)p[3]/100.0;//a
		printf("A=%4.2f \n",ret);
		
		break;

	case 3:
		wlen = write(fd, "\xB2\xC0\xA8\x01\x01\x00\x1C", 7);
		printf("sent B2C0A80101001C\n");//read v
		if (wlen != 7) {
			printf("Error from write: %d, %d\n", wlen, errno);
		}
		tcdrain(fd);    /* delay for output */
		do
		{	
			length = read(fd, buffer + rlength, 1);
			checksum += buffer[rlength];		
			rlength += length;
		} while(rlength < 7);
			checksum -= buffer[6];
		
			printf("%x,%x \n",buffer[6],checksum);
			printf("%x,%x,%x,%x,%x,%x,%x \n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6]);
		if (f[0] != 0xA2 || buffer[6] != (checksum & 0xFF)){
			printf("checksum error! \n");
			break;		
		}
		buffer[rlength] = '\0';
		ret = (float)((p[1]<<8)+(p[2]));//W
		printf("W=%d \n",(int)ret);
		
		break;
	case 4:
		wlen = write(fd, "\xB3\xC0\xA8\x01\x01\x00\x1D", 7);
		printf("sent B3C0A80101001D\n");//read v
		if (wlen != 7) {
			printf("Error from write: %d, %d\n", wlen, errno);
		}
		tcdrain(fd);    /* delay for output */
		do
		{	
			length = read(fd, buffer + rlength, 1);
			checksum += buffer[rlength];		
			rlength += length;
		} while(rlength < 7);
			checksum -= buffer[6];
		
			printf("%x,%x \n",buffer[6],checksum);
		if (buffer[0] != 0xA3 || buffer[6] != (checksum & 0xFF)){
			printf("checksum error! \n");
			break;		
		}
		buffer[rlength] = '\0';
		ret = (float)((p[1]<<16)+(p[2]<<8)+(p[3]));//wh
		printf("WH=%d \n",(int)ret);
		
		break;

	default:
		printf("error type");
		return -1.0;
	}
	tcflush(fd, TCIFLUSH);
	return ret; 
}

int main(int argc,char **argv)
{
	char *portname = "/dev/ttyUSB0";
	int fd;
	int a =1;
	if(argc < 2){
		printf(" Default Port: /dev/ttyUSB0 \n");
		fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
		a = 1;
	}else{
		printf("Port set to %s \n",argv[1] );
		fd = open(argv[1], O_RDWR | O_NOCTTY | O_SYNC);
		a = atoi(argv[2]);
	}

	if (fd < 0) {
		printf("Error opening %s: %s\n", portname, strerror(errno));
	return -1;
	}
	/*baudrate 115200, 8 bits, no parity, 1 stop bit */
	set_interface_attribs(fd, B9600);
	//set_mincount( fd ,7 );

	tcflush(fd, TCIFLUSH);
	//printf("%d",a);
	return powerread(fd,a);

}

