<!DOCTYPE html>
<html lang="zh">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>提交状态</title>
	<meta charset="utf-8">
	<link rel="stylesheet" type="text/css" href="./login/main.css">
</head>

<body>
<div class="acenter">
	<h1>提交状态</h1>
	<fieldset class="login">
		<legend>提交情况</legend>
		<form>
			<?php
			session_start();
			
			$number=$_SESSION['number'];//学号
			$name=$_SESSION['name'];//姓名
			$specialty=$_SESSION['specialty'];//专业
			$class=$_SESSION['class'];//班级
			$email=$_SESSION['email'];//邮箱
			$cellphone=$_SESSION['cellphone'];//手机			
			
			include 'conn.php';
			$sql = "select * from `submitstatus` WHERE `number` =$number";
			$b=mysql_query($sql);
			$row=mysql_fetch_array($b);
			print_r($row);
			?>
		</form>
	</fieldset>
</div>

<div class="loading-bar hide"><div></div></div>
</body></html>