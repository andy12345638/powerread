#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


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

int main(){
	int fd;

	float test;
	fd=open("/sys/bus/w1/devices/28-0316c3047fff/w1_slave",O_RDONLY | O_NOCTTY | O_SYNC);
	test = tempread(fd);
	printf("%5.3f",test);


	close(fd);


return 0;
}


