# powermonitor
rpi3 with DS18B20 & PZEM-004T(V1.0)  

# 材料
1. Raspberry PI 3 Model B  
2. Sandisk sd card 32G   
3. 110V to USB 充電器 2.5A  
4. usb 充電線 30cm   
5. DS18B20 溫度感應器 1米*3條   
6. 其他焊接材料(洞洞板、電阻、杜邦頭)  
7. PZEM-004T(V1.0) 交流計量模組  
共計 $4500 內  

# 架構
1. powerread.c   
用於讀取PZEM-004T(V1.0)  
使用方式  
`gcc powerread.c -o powerread`  
`./powerread /dev/tty/USB0 1`  
1 為讀取電壓;2 電流 ;3 瞬時功率;4 累積電量。  

2. tempc.c  
用於讀取DS18B20  
使用方式  
`gcc tempc.c -o tempc`  
`./tempc`  

3. mysql.c
用於將資料送入 mysql server  
使用方式  
`gcc mysql.c -O3 -lmysqlclient -I/usr/include/mysql/ -o mysql`  
`./mysql`  
凡有使用到mysql.h的資料編譯時都需要補上 `-lmysqlclient -I/usr/include/mysql/`  

4. p2t2m.c  
用於讀取兩個電量資料，兩個溫度資料並送入 mysql server。  
`gcc p2t2m.c -o p2t2m -O3 -lmysqlclient -I/usr/include/mysql/`  
`./p2t2m`  

# mysql
`sudo apt-get install mysql-server mysql-client`  
  
`mysql -u root -p`  
  
`CREATE DATABASE powermonitor;`  
  
`USE powermonitor;`  
  
`CREATE TABLE power (`  
`timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, `  
`v1 DECIMAL(4,1),`  
`v2 DECIMAL(4,1),`  
`v3 DECIMAL(4,1),`  
`a1 DECIMAL(5,2),`  
`a2 DECIMAL(5,2),`  
`a3 DECIMAL(5,2),`  
`w1 SMALLINT(6),`  
`w2 SMALLINT(6),`  
`w3 SMALLINT(6),`  
`wh1 INT(11) UNSIGNED,`  
`wh2 INT(11) UNSIGNED,`  
`wh3 INT(11) UNSIGNED,`  
`temp1 decimal(5,3), `  
`temp2 decimal(5,3), `  
`temp3 decimal(5,3),`  
`PRIMARY KEY (timestamp)`  
`);`  
  
---

`use mysql;`  
  
`INSERT INTO user(host,user,password) VALUES('%.%.%.%','rpi',password('paswd'));`  
  
`GRANT SELECT,INSERT,UPDATE,CREATE ON powermonitor.* TO 'rpi' IDENTIFIED BY 'paswd';`  

`FLUSH PRIVILEGES;`  
  
---  
