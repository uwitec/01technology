<?php /* Smarty version 2.6.18, created on 2009-08-09 14:02:32
         compiled from tpl-news-desc.dwt */ ?>
﻿<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "library/meta.lbi", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "library/style.lbi", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
</head>

<body>
<div id="body">
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "library/header.lbi", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
  <div class="bg">
  <div class="path_url">当前位置:<a href="/">首页</a> &gt; <a href="/index.php/News">最新消息</a> &gt; <?php echo $this->_tpl_vars['news'][0]['title']; ?>
</div>  
  <div class="page_title">
  <h2>最新消息</h2>
  <h3>NEWS</h3></div>
  <div class="split_lint"></div>
  <div style="width:600px; min-height:350px; margin:0px auto;">
   <h2 style="text-align:center; width:600px; margin:0px; color:#85817B;"><?php echo $this->_tpl_vars['news'][0]['title']; ?>
</h2>
   <p style="text-align:center; color:#85817B; font-size:10px;"><?php echo $this->_tpl_vars['news'][0]['created']; ?>
</p>
   <p style="width:600px; text-indent:2em;"><?php echo $this->_tpl_vars['news'][0]['content']; ?>
</p>
  </div>
  <div class="split_lint"></div>
  <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "library/footer.lbi", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
  <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "library/contactus.lbi", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
  </div> 
 </div>
</div>
<div class="nav_bg">
 <div id="navigation">
  <div id="menu">
  <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "library/menu.lbi", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
  </div>
  <div class="heard"></div>
  <div class="nav_pic nav_contactus"></div>
  <div class="left_line other_left_line"></div>
  <div id="left_contact"></div>
 </div>
</div>

</body>

</html>