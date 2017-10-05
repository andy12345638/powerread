#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <mysql.h>
//gcc test8.c -O3 -lmysqlclient -I/usr/include/mysql/


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
	int wlen;
	char buffer[8];
	int checksum = 0;
	unsigned char   *p = buffer;
	int rlength = 0;
	int length;
	char *loc; 
	float ret;
	switch(a){
	case 1:
		wlen = write(fd, "\xB0\xC0\xA8\x01\x01\x00\x1A", 7);
		printf("sent B0C0A80101001A\n");//read v
		if (wlen != 7) {
			printf("Error from write: %d, %d\n", wlen, errno);
		}
		tcdrain(fd);    /* delay for output */
		//usleep(50000);	
		//rlength = 0;
		do
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
		if (buffer[0] != 0xA2 || buffer[6] != (checksum & 0xFF)){
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

float tempread(int fd){
	char buf[100];
	int length;
	char *loc; 
	char temp[4];
	int temp1;
	float temp2;

	length = read(fd,buf,99);
	buf[length] = '\0';

	loc = strchr(buf,'t');

	memcpy(temp,loc+2,2);
	temp[2] = '\0';
	temp1 = atoi(temp);
	memcpy(temp,loc+4,3);
	temp[3] = '\0';
	temp2 =temp1 + atoi(temp)/1000.0;

	printf("%5.3f\n",temp2);
	return temp2;
}


int main()
{
	MYSQL *mysql = NULL;
	mysql = mysql_init(mysql);
	//char *portname = "/dev/ttyUSB0";
	int fdp1;
	int fdp2;
	int fdt1;
	int fdt2;


	printf(" Default Port: /dev/ttyUSB0 \n");
	fdp1 = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_SYNC);

	if (fdp1 < 0) {
		printf("Error opening %s: %s\n", "/dev/ttyUSB0", strerror(errno));
	return -1;
	}

	printf(" Default Port: /dev/ttyUSB1 \n");
	fdp2 = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_SYNC);


	if (fdp2 < 0) {
		printf("Error opening %s: %s\n", "/dev/ttyUSB1", strerror(errno));
	return -1;
	}
	
	fdt1=open("/sys/bus/w1/devices/28-0316c3047fff/w1_slave",O_RDONLY | O_NOCTTY | O_SYNC);
	if (fdt1 < 0) {
		printf("Error opening %s: %s\n", "28-0316c3047fff", strerror(errno));
	return -1;
	}

	fdt2=open("/sys/bus/w1/devices/28-0316c339fdff/w1_slave",O_RDONLY | O_NOCTTY | O_SYNC);
	if (fdt2 < 0) {
		printf("Error opening %s: %s\n", "28-0316c339fdff", strerror(errno));
	return -1;
	}


	/*baudrate 115200, 8 bits, no parity, 1 stop bit */
	set_interface_attribs(fdp1, B9600);
	set_interface_attribs(fdp2, B9600);
	//set_mincount( fd ,7 );

	tcflush(fdp1, TCIOFLUSH);
	tcflush(fdp2, TCIOFLUSH);
	usleep(1000);
	//printf("%d",a);

	if(!mysql_real_connect(mysql,"127.0.0.1","rpi","12345678","powermonitor",3306,NULL,0))
	printf("MYSQL Init Error\n");
	else
	printf("MYSQL Init OK\n");
	
	char query[100];
	sprintf(query,"INSERT INTO power (v1,a1,w1,wh1,v2,a2,w2,wh2,temp1,temp2) VALUES(%4.1f,%4.2f,%d,%d,%4.1f,%4.2f,%d,%d,%5.3f,%5.3f)",
		powerread(fdp1,1),powerread(fdp1,2),(int)powerread(fdp1,3),(int)powerread(fdp1,4),
		powerread(fdp2,1),powerread(fdp2,2),(int)powerread(fdp2,3),(int)powerread(fdp2,4),
		tempread(fdt1),tempread(fdt2));
	//printf("VALUES(%4.1f,%4.2f,%d,%d,%4.1f,%4.2f,%d,%d\n)",powerread(fd,1),powerread(fd,2),(int)powerread(fd,3),(int)powerread(fd,4),powerread(fd2,1),powerread(fd2,2),(int)powerread(fd2,3),(int)powerread(fd2,4));

	if(mysql_query(mysql,query))
	printf("MYSQL Query Error!\n");
	else
	printf("MYSQL Query Done!\n");
	

	close(fdp1);
	close(fdp2);
	close(fdt1);
	close(fdt2);
	return 0;

}

