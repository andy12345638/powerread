#p2t2m.c 2017/09/30 change to c> mysql > r-plot > php
#install.packages("showtext")
library(showtext)
library(magrittr)
library(RMySQL)
conn <- dbConnect(MySQL(),host="192.168.2.60", dbname = "powermonitor", username="rpi", password="12345678")
  sqldf = dbGetQuery(conn,"select * from powermonitor.power where timestamp > (now() - INTERVAL 72 HOUR) LIMIT 4320;")#3days
  daysqldf = dbGetQuery(conn,"select * from powermonitor.power where  DATE(timestamp) = CURDATE() and hour(timestamp)  BETWEEN '05' AND '20';")#today05~20
  ghost_w1=dbGetQuery(conn,"select round(avg(w1),0) from powermonitor.power where   hour(timestamp)=hour(now()) and minute(timestamp)=minute(now()) group by date_format(timestamp,'%H:%i');")#group w1 by minute
  ghost_hs1=dbGetQuery(conn,"select date_format(timestamp,'%H:%i') as t,avg(w1) as w1 from powermonitor.power group by date_format(timestamp,'%H:%i');")#group w1 by minute
  sqldaykwh1=dbGetQuery(conn,"select from_days(to_days(timestamp)) as date,max(wh1) as max, min(wh1) as min ,max(wh1) - min(wh1) as daywh from powermonitor.power group by from_days(to_days(timestamp));")#everyday point
  ghost_w2=dbGetQuery(conn,"select round(avg(w2),0) from powermonitor.power where   hour(timestamp)=hour(now()) and minute(timestamp)=minute(now()) group by date_format(timestamp,'%H:%i');")#group w1 by minute
  ghost_hs2=dbGetQuery(conn,"select date_format(timestamp,'%H:%i') as t,avg(w2) as w2 from powermonitor.power group by date_format(timestamp,'%H:%i');")#group w1 by minute
  sqldaykwh2=dbGetQuery(conn,"select from_days(to_days(timestamp)) as date,max(wh2) as max, min(wh2) as min ,max(wh2) - min(wh2) as daywh from powermonitor.power group by from_days(to_days(timestamp));")#everyday point
dbDisconnect(conn)  
showtext_auto(enable = TRUE)
font_add("cwTeXYen","cwyen.ttf")

sqldf=as.data.frame(sqldf,stringsAsFactors=FALSE)
sqldf$timestamp%<>%strptime(., "%Y-%m-%d %H:%M:%S")
daysqldf=as.data.frame(daysqldf,stringsAsFactors=FALSE)
daysqldf$timestamp%<>%strptime(., "%Y-%m-%d %H:%M:%S")
ghost_hs1%<>%as.data.frame(.,stringsAsFactors=FALSE)
ghost_hs1$t%<>%strptime(., "%H:%M")
sqldaykwh1%<>%as.data.frame(.,stringsAsFactors=FALSE)
sqldaykwh1$date%<>%as.Date(.,origin="1970-01-01")

ghost_hs2%<>%as.data.frame(.,stringsAsFactors=FALSE)
ghost_hs2$t%<>%strptime(., "%H:%M")
sqldaykwh2%<>%as.data.frame(.,stringsAsFactors=FALSE)
sqldaykwh2$date%<>%as.Date(.,origin="1970-01-01")

msg=paste(
paste("現在版本",tail(sqldf$timestamp,1)),
paste("現在時間", Sys.time()),
paste("p1即時功率", tail(sqldf$w1,1),"w/",tail(sqldf$a1*sqldf$v1,1),"w/",ghost_w1,"w1"),
paste("p2即時功率", tail(sqldf$w2,1),"w/",tail(sqldf$a2*sqldf$v2,1),"w/",ghost_w2,"w2"),

paste("現在t1溫度", tail(sqldf$temp1,1),"度"),
paste("現在t2溫度", tail(sqldf$temp2,1),"度"),
paste("p1現在功率因數", tail(replace(sqldf$w1/(sqldf$a1*sqldf$v1), is.na(sqldf$w1/(sqldf$a1*sqldf$v1)), 0),1),"%"),
paste("p2現在功率因數", tail(replace(sqldf$w2/(sqldf$a2*sqldf$v2), is.na(sqldf$w2/(sqldf$a2*sqldf$v2)), 0),1),"%"),

paste("p1平均功率因數", mean(sqldf$w1/(sqldf$a1*sqldf$v1),na.rm = T),"%"),
paste("p2平均功率因數", mean(sqldf$w2/(sqldf$a2*sqldf$v2),na.rm = T),"%"),

paste("p1今天耗電量", tail(sqldaykwh1$daywh/1000,1),"度"),
paste("p2今天耗電量", tail(sqldaykwh2$daywh/1000,1),"度"),

paste("系統已運作", length(sqldaykwh1$date),"天"),
paste("p1累積耗電量", max(sqldf$wh1)/1000,"度"),
paste("p2累積耗電量", max(sqldf$wh2)/1000,"度"),

paste("p1平均每日耗電", max(sqldf$wh1)/(1000*length(sqldaykwh1$date)),"度"),
paste("p2平均每日耗電", max(sqldf$wh2)/(1000*length(sqldaykwh2$date)),"度"),
sep="\r\n")
write.table(msg,"/home/pi/powermonitor/R/msg.csv",row.names = FALSE)
#write.table(msg,"~/msg.csv",row.names = FALSE)


png("/home/pi/powermonitor/R/p1v.png",width = 1200, height = 400)
plot(sqldf$timestamp,sqldf$v1,type="l",main="p1電壓",xlab="Time",ylab="電壓(V)",family ="cwTeXYen" )
abline(h=mean(sqldf$v1),col="blue")
dev.off()
png("/home/pi/powermonitor/R/p2v.png",width = 1200, height = 400)
plot(sqldf$timestamp,sqldf$v2,type="l",main="p2電壓",xlab="Time",ylab="電壓(V)")
abline(h=mean(sqldf$v1),col="blue")
dev.off()

#plot(sqldf$timestamp,sqldf$a1,type="l",main="電流",xlab="Time",ylab="電流(A)")

png("/home/pi/powermonitor/R/p1wd.png",width = 1200, height = 400)
plot(daysqldf$timestamp,daysqldf$w1,type="l",main="p1今日有功功率",xlab="Time",ylab="有功功率(w)")
dev.off()
png("/home/pi/powermonitor/R/p2wd.png",width = 1200, height = 400)
plot(daysqldf$timestamp,daysqldf$w2,type="l",main="p2今日有功功率",xlab="Time",ylab="有功功率(w)")
dev.off()

#plot(daysqldf$timestamp,daysqldf$temp1,type="l",main="t1今日溫度",xlab="Time",ylab="溫度")


#plot(sqldf$timestamp,sqldf$v1*sqldf$a1,type="l",main="視在功率",xlab="Time",ylab="視在功率(V*A)")
png("/home/pi/powermonitor/R/p1w.png",width = 1200, height = 400)
plot(sqldf$timestamp,sqldf$w1,type="l",main="p1有功功率",xlab="Time",ylab="有功功率(W)")
dev.off()
png("/home/pi/powermonitor/R/p2w.png",width = 1200, height = 400)
plot(sqldf$timestamp,sqldf$w2,type="l",main="p2有功功率",xlab="Time",ylab="有功功率(W)")
dev.off()

png("/home/pi/powermonitor/R/t1.png",width = 1200, height = 400)
plot(sqldf$timestamp,sqldf$temp1,type="l",main="p1現在設備溫度",xlab="Time",ylab="溫度")
dev.off()
png("/home/pi/powermonitor/R/t2.png",width = 1200, height = 400)
plot(sqldf$timestamp,sqldf$temp2,type="l",main="p2現在設備溫度",xlab="Time",ylab="溫度")
dev.off()


#plot(sqldf$timestamp,sqldf$temp1,type="l",ylim=c(min(sqldf$temp2)*0.98,max(sqldf$temp1)*1.02),main="混合溫度",xlab="Time",ylab="溫度")
#par(new = T)
#plot(sqldf$timestamp,sqldf$temp2,type="l",ylim=c(min(sqldf$temp2)*0.98,max(sqldf$temp1)*1.02), axes=F, xlab=NA, ylab=NA, cex=1.2,col="green")

#plot(sqldf$timestamp,sqldf$wh1/1000,type="l",main="累計千瓦時",xlab="Time",ylab="度(KWH)")
#plot(sqldf$timestamp,sqldf$w1/(sqldf$a1*sqldf$v1),type="l",main="功率因數",xlab="Time",ylab="功率因數(%)")
#abline(h=c(0.7,0.9,1))
#abline(h=mean(sqldf$w1/(sqldf$a1*sqldf$v1),na.rm = T),col="blue")

#par(mar = c(5,5,2,5))
#plot(sqldf$timestamp,sqldf$wh1/1000,type="l",main="功率與累計產量",xlab="Time",ylab="度(KWH)")
#par(new = T)
#plot(sqldf$times,sqldf$v1*sqldf$a1,type="l", axes=F, xlab=NA, ylab=NA, cex=1.2,col="green")
#axis(side = 4)
#mtext(side = 4, line = 3, 'W=A*V')

png("/home/pi/powermonitor/R/p1kwh.png",width = 1200, height = 400)
plot(as.Date(sqldaykwh1$date,origin="1970/01/01"),sqldaykwh1$daywh/1000,main="p1每日用電度數",xlab="Time",ylab="度(KWH)",ylim=c(0,max(sqldaykwh1$daywh/1000,na.rm = T)+0.2))
abline(h=mean(sqldaykwh1$daywh/1000,na.rm = T))
stdevs <- mean(sqldaykwh1$daywh/1000,na.rm = T) + c(-1, +1) * sd(sqldaykwh1$daywh/1000,na.rm = T)
abline(h=stdevs, lty=2, lwd=4, col="blue")
dev.off()
png("/home/pi/powermonitor/R/p2kwh.png",width = 1200, height = 400)
plot(as.Date(sqldaykwh2$date,origin="1970/01/01"),sqldaykwh2$daywh/1000,main="p2每日用電度數",xlab="Time",ylab="度(KWH)",ylim=c(0,max(sqldaykwh2$daywh/1000,na.rm = T)+0.2))
abline(h=mean(sqldaykwh2$daywh/1000,na.rm = T))
stdevs <- mean(sqldaykwh2$daywh/1000,na.rm = T) + c(-1, +1) * sd(sqldaykwh2$daywh/1000,na.rm = T)
abline(h=stdevs, lty=2, lwd=4, col="blue")
dev.off()

png("/home/pi/powermonitor/R/p1d.png",width = 1200, height = 400)
plot(ghost_hs1,type="l",col="blue")
dev.off()
png("/home/pi/powermonitor/R/p2d.png",width = 1200, height = 400)
plot(ghost_hs2,type="l",col="blue")
dev.off()
