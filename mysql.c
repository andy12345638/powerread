#include <mysql.h>
#include <stdio.h>

int main(){
MYSQL *mysql;


//mysql_library_init(0,NULL,NULL);

mysql = mysql_init(mysql);

if(!mysql_real_connect(mysql,"192.168.1.133","rpi","12345678","powermonitor",3306,NULL,0))
printf("MYSQL Init Error\n");
else
printf("MYSQL Init OK\n");


if(mysql_query(mysql,"INSERT INTO power (v1) VALUES(110.1)"))
printf("error\n");
else
printf("OK\n");






mysql_close(mysql); 
return 0;
}


