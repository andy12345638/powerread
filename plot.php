<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>電力監控</title>
<meta http-equiv="refresh" content="60" />
</head>


<body>
<h1><center>電力監控</center></h1> <br>
<hr>

<?php

// Use fopen function to open a file
$file = fopen("msg.csv", "r");

// Read the file line by line until the end
while (!feof($file)) {
$value = fgets($file);
print "" . $value . "<br>";
}

// Close the file that no longer in use
fclose($file);

?>

<h2><center>1號裝置三日電壓</center></h2> <br>
<center><img src="p1v.png" style="width:1200px;height:400px;"></center><br>
<br>
<h2><center>2號裝置三日電壓</center></h2> <br>
<center><img src="p2v.png" style="width:1200px;height:400px;"></center><br>
<br>

<h2><center>1號裝置今日有功功率</center></h2> <br>
<center><img src="p1wd.png" style="width:1200px;height:400px;"></center><br>
<br>
<h2><center>2號裝置今日有功功率</center></h2> <br>
<center><img src="p2wd.png" style="width:1200px;height:400px;"></center><br>
<br>

<h2><center>1號裝置三日有功功率</center></h2> <br>
<center><img src="p1w.png" style="width:1200px;height:400px;"></center><br>
<br>
<h2><center>2號裝置三日有功功率</center></h2> <br>
<center><img src="p2w.png" style="width:1200px;height:400px;"></center><br>
<br>

<h2><center>1號裝置三日溫度</center></h2> <br>
<center><img src="t1.png" style="width:1200px;height:400px;"></center><br>
<br>
<h2><center>2號裝置三日溫度</center></h2> <br>
<center><img src="t2.png" style="width:1200px;height:400px;"></center><br>
<br>

<h2><center>1號裝置每日用電度數</center></h2> <br>
<center><img src="p1kwh.png" style="width:1200px;height:400px;"></center><br>
<br>
<h2><center>2號裝置每日用電度數</center></h2> <br>
<center><img src="p2kwh.png" style="width:1200px;height:400px;"></center><br>
<br>

<h2><center>1號裝置每日用電曲線</center></h2> <br>
<center><img src="p1d.png" style="width:1200px;height:400px;"></center><br>
<br>
<h2><center>2號裝置每日用電曲線</center></h2> <br>
<center><img src="p2d.png" style="width:1200px;height:400px;"></center><br>
<br>

</body>
</html>
