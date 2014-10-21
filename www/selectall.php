<!DOCTYPE html>
<html lang="zh">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<meta http-equiv="refresh" content="10">
	<title>提交状态</title>
	<meta charset="utf-8">
	<link rel="stylesheet" type="text/css" href="./login/main.css">
	<style type="text/css">

table.altrowstable {

    font-family: verdana,arial,sans-serif;

    font-size:11px;

    color:#333333;

    border-width: 1px;

    border-color: #a9c6c9;

    border-collapse: collapse;

}

table.altrowstable th {

    border-width: 1px;

    padding: 8px;

    border-style: solid;

    border-color: #a9c6c9;
	
	

}

table.altrowstable td {

    border-width: 1px;

    padding: 8px;

    border-style: solid;

    border-color: #a9c6c9;

}

.oddrowcolor{

    background-color:#d4e3e5;

}

.evenrowcolor{

    background-color:#c3dde0;

}

</style>
</head>	
<body>
<div class="acenter">
	<br/>
	<br/>
	<h1>提交状态</h1>
			<?php
			include 'conn.php';
			$sql = "select * from `experiment` order by `score` desc ";
			$b=mysql_query($sql);
			while($row=mysql_fetch_array($b))
			{
			echo '<table class="altrowstable" id="alternatecolor" >';
			echo	'<tr>';
			echo		'<th>姓名</th><th>学号</th><th>提交状况</th><th>提交详情</th>';
			echo	'</tr>';
			echo	'<tr>';
			echo	'<td align="center">';
						$sql_name="select `name` from `member` where `number`='$row[1]'";
						$d=mysql_query($sql_name);
						$name=mysql_fetch_array($d);
						echo $name[0];
					echo '</td>';
			echo 	'<td align="center">';
					echo $row[1];
			echo 	'</td>';
			echo	'<td >';echo "提交次数:";
							echo $row[2];"<br/>"."<br/>";
							echo "是否通过:";
							echo $row[4]."<br/>"."<br/>";
							echo "得分:";
							echo $row[7]."<br/>"."<br/>";
							echo '</td>';
			echo	'<td align="center">';
					$sql_time="select * from `experiment` where `number`='$row[1]' order by `sub_time` desc";
					$c=mysql_query($sql_time);
					while($sub_time=mysql_fetch_array($c)){
						echo $sub_time[3]."<br/>"."是否通过".$sub_time[4]."分数:".$sub_time[7];
						
					}
					echo '</td>';
			echo	'</tr>';
			}
			
			//print_r($row);
			//echo "<script>location:reload();</script>";
			?>
</div>

<div class="loading-bar hide"><div></div></div>

<script type="text/javascript">

function altRows(id){

    if(document.getElementsByTagName){  

        

        var table = document.getElementById(id);  

        var rows = table.getElementsByTagName("tr"); 

         

        for(i = 0; i < rows.length; i++){          

            if(i % 2 == 0){

                rows[i].className = "evenrowcolor";

            }else{

                rows[i].className = "oddrowcolor";

            }      

        }

    }

}

window.onload=function(){

    altRows('alternatecolor');

}

</script>

</body>

</html>
