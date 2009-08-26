<?php /* Smarty version 2.6.18, created on 2009-07-29 11:40:30
         compiled from tpl-admin-changepsw.html */ ?>
﻿<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>后台管理界面</title>
<link href="<?php echo CSS_DIR ?>/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
</style>
</head>
<body>
<div id="body"> <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'tpl-admin-header.html', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
  <div id="content"> <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'tpl-admin-menu.html', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
    <div id="ct_rt">
      <form action="/index.php/Admin/ChangePSWdo" method="post" >
      <table class="table_list">
        <thead>
        <tr><td></td><td></td></tr>
        </thead>
        <tbody>
          <tr>
	        <td class="tb50">原密码</td>
	        <td class="tb250"><input type="password" name="password" /></td>
	      </tr>
	      <tr>
	        <td class="tb100">新密码</td>
	        <td class="tb100"><input type="password" name="passwordnew" /></td>
	      </tr>
	      <tr>
	        <td class="tb100">再次输入新密码</td>
	        <td class="tb100"><input type="password" name="pssswordrepet" /></td>
	      </tr>
	      <tr>
	        <td class="tb50">&nbsp</td>
	        <td class="tb50"><input type="submit" name="submit" value="提交" /></td>
	      </tr>
    	</tbody>
	  </table>
	  </form>
  </div>
</div>
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'tpl-admin-footer.html', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?> </div>
</body>
</html>