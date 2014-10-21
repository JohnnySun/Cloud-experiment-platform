<?php
$db = mysql_connect("localhost","root","whoami") or die("error");
	mysql_select_db("test") or die("error");
	mysql_query("SET NAMES 'UTF8'");
?>