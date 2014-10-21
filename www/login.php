<?php
	session_start();
	if(!empty($_POST['name'])||!empty($_POST['number'])){
	header("content-type:text/html;charset=utf-8");
	//var_dump($_POST);
	$name = $_POST['name'];
	$number = $_POST['number'];

	$db = mysql_connect("localhost","root","whoami") or die("error");
	mysql_select_db("test") or die("error");
	mysql_query("SET NAMES 'UTF8'");
	$sql = "select * from `member` WHERE `number` =$number";
	$a=mysql_query($sql);
	$row=mysql_fetch_array($a);
	
	//print_r($row);
	//echo $row[2];
	$_SESSION['number']=$row[1];//学号
	$_SESSION['name']=$row[2];//姓名
	$_SESSION['specialty']=$row[3];//专业
	$_SESSION['class']=$row[4];//班级
	$_SESSION['email']=$row[5];//邮箱
	$_SESSION['cellphone']=$row[6];//手机
	$result = mysql_query($sql);
	$query = mysql_fetch_array($result);
	
	//var_dump($query['name']);

	if ($query['name'] == $name){
		header("location:upload.html");	
	}else{
		header("location:login.html");
	}
}else{
	header("location:login.html");
}


 ?>