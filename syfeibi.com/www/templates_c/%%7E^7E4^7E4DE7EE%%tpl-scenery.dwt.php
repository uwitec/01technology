<?php /* Smarty version 2.6.18, created on 2009-08-10 15:22:40
         compiled from tpl-scenery.dwt */ ?>
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
  <div class="path_url">当前位置:<a href="/">首页</a> &gt; 公司内景</div>  
  <div class="page_title">
  <h2>公司内景</h2>
  <h3>COMPANY SCENERY</h3></div>
  <div id="scenery" class="div_ct min-height lightbox">
   <ul>
    <li><a href="<?php echo SCENERY_DIR ?>/1.jpg"><img src="<?php echo SCENERY_DIR ?>/1.jpg" alt=""/></a></li><li><a href="<?php echo SCENERY_DIR ?>/2.jpg"><img src="<?php echo SCENERY_DIR ?>/2.jpg" alt=""/></a></li>
   </ul>
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