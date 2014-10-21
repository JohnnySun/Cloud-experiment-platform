<?php
	session_start();
	if($_SESSION['number']){
		if(@is_uploaded_file($_FILES['upfile']['tmp_name'])){
			$number=$_SESSION['number'];//学号
			$name=$_SESSION['name'];//姓名
			$specialty=$_SESSION['specialty'];//专业
			$class=$_SESSION['class'];//班级
			$email=$_SESSION['email'];//邮箱
			$cellphone=$_SESSION['cellphone'];//手机
		//文件夹名字
			date_default_timezone_set("PRC");
			$aaa=$number.'_'.date("YmdHis");
			//echo $aaa;
			//exit();
		//文件操作
			@$tmpfile=$_FILES['upfile']['tmp_name'];//获取是上传文件到系统临时文件名
			@$oldname=$_FILES['upfile']['name'];   //获取你上传前的文件名
			$fiearr=explode(".",$oldname);       //将原文件名分成数组
			$key=count($fiearr)-1;                //计算出最后一个扩展名的主键
			$fie_extend=$fiearr[$key];             //列出上传文件的扩展名
			$fie_extend=strtolower($fie_extend);  //将扩展名统一为小写
			if($fie_extend=="c"||$fie_extend=="cpp"){    //限制上传为c的文件
				$filenews=$aaa.".cpp";
				$filedir="/var/www/upload/".$filenews;       //目标文件的路径(包含文件名)
				$a=move_uploaded_file($tmpfile,$filedir);// 把临时移动到目标文件
				$code_path=(string)$filedir;
				$type="cpp"; 
				$user=(string)$_SESSION['number'];
				$mail=(string)$_SESSION['email'];
				$path="/usr/local/Web_Compile/save2list.py"." ".$code_path." ".$type." ".$user." ".$mail;
				$fp=popen($path,"r");
				echo "上传成功，请选择别的文件";
				//update不知道写不写;
				header("location:selectall.php");	
			}else{
				echo "请选择正确的文件格式";
			}
		}else{
			header("location:login.html");
			}
	}else{
			header("location:login.html");
	}
