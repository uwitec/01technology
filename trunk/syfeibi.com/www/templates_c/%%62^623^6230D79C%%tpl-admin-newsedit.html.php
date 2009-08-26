<?php /* Smarty version 2.6.18, created on 2009-08-23 12:22:31
         compiled from tpl-admin-newsedit.html */ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>后台管理界面  新闻编辑/添加</title>
<link href="<?php echo CSS_DIR ?>/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
</style>
<script type="text/javascript" src="<?php echo FCK_DIR ?>/fckeditor.js"></script>
<script type="text/javascript">
window.onload = function()
{
	// Automatically calculates the editor base path based on the _samples directory.
	// This is usefull only for these samples. A real application should use something like this:
	// oFCKeditor.BasePath = '/fckeditor/' ;	// '/fckeditor/' is the default value.
	var sBasePath = '<?php echo FCK_DIR ?>/' ;

	var oFCKeditor = new FCKeditor( 'content' ) ;
	oFCKeditor.BasePath	= sBasePath ;
	oFCKeditor.Height	= 380 ;
//	oFCKeditor.Config['ToolbarStartExpanded'] = false ;
	oFCKeditor.ToolbarSet	= 'MyToolbar' ;
	oFCKeditor.ReplaceTextarea() ;
}
</script>
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
    <div id="ct_rt" style=" height:inherit; overflow:scroll;">
      <form action="/index.php/Admin/Newseditdo" method="post">
        <table class="table_list">
         <thead>
          <tr><td></td><td></td></tr>
         </thead>
          <tr>
            <td>标题：</td>
            <td align="left"><input class="tb350" type="text" name="title" value="<?php echo $this->_tpl_vars['news']['title']; ?>
" /></td>
          </tr>
          <tr>
            <td>内容：</td>
            <td align="left"><textarea name="content"><?php echo $this->_tpl_vars['news']['content']; ?>
</textarea></td>
          </tr>
          <tr>
            <td>类型：</td>
            <td align="left"><select name="type"><option value="0" <?php if (( $this->_tpl_vars['news']['type'] == 0 )): ?>selected<?php endif; ?>>新闻</option><option value="1" <?php if (( $this->_tpl_vars['news']['type'] == 1 )): ?>selected<?php endif; ?>>公告</option></select></td>
          </tr>
          <tr>
            <td>&nbsp;</td>
            <td align="left"><input type="hidden" name="newsid" value="<?php echo $this->_tpl_vars['news']['newsid']; ?>
" />
              <input type="submit" name="submit" value="提交" />
              <input type="reset" name="reset" value="重置" />
              <input type="button" name="cannel" value="返回" onclick="javascript:history.back(-1);" /></td>
          </tr>
        </table>
      </form>
    <div style="clear:both;"></div>  
    </div>
  </div>
  <div style="clear:both;"></div>
  <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'tpl-admin-footer.html', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?> </div>
</body>
</html>