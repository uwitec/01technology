<?php /* Smarty version 2.6.18, created on 2009-07-29 15:42:02
         compiled from tpl-adminlogin.html */ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>辽宁万维医药网站管理平台</title>
<link type="text/css" href="<?php echo CSS_DIR ?>/admin.css" rel="stylesheet" />
</head>
<body style="background:#0a7705;">
<div class="login_bg">
 <div class="login_ct">
  <div class="login_ct_title"></div>
  <form id="theForm" action="/index.php/Admin/login" method="post">
  <div class="login_ct_form">
   <ul>
    <li><lable style="line-height:26px; height:26px; vertical-align:middle;">管理员：</lable><input type="text" name="name" class="input_login" /></li>
    <li><lable>密&nbsp;&nbsp;&nbsp;&nbsp;码：</lable><input type="password" name="pwd" class="input_pwd" /></li>
	<li><input type="submit" class="btn" value="登陆"/> <input type="reset" class="btn" value="重填"/></li>    
   </ul>
  </div>
  </form>
</div>
</body>
</html>