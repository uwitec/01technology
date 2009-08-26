<?php defined('APP_DIR') or die(); // 阻止对模板文件的直接访问 ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>欢迎使用 FleaPHP</title>
<link rel="stylesheet" type="text/css" href="css/style.css">
</head>
<body>
<h1>欢迎使用 FleaPHP !</h1>
<p>本页内容通过 FleaPHP 应用程序动态产生。</p>
<p>如果你想要修改该页内容，可以编辑文件：</p>
<code><?php echo h(__FILE__); ?></code>
<p>该页用于控制器 <?php echo $this->_controllerName; ?> 的 <?php echo $this->_actionName ?> 动作。要修改该控制器，可以编辑文件：</p>
<code><?php echo h($code_filename); ?></code>
<p><br />
  <?php
// 确保可以获得载入 FLEA.php 时的计时器值
global $___fleaphp_loaded_time;
?>
  页面生成耗时：<?php printf('%0.4f', microtime_float(microtime()) - microtime_float($___fleaphp_loaded_time)); ?> 秒 <br />
  FleaPHP 版本：<?php echo FLEA_VERSION; ?> </p>
</body>
</html>
