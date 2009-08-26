<?php /* Smarty version 2.6.18, created on 2009-07-29 11:41:07
         compiled from tpl-admin-goodsedit.html */ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>后台管理界面</title>
<link href="<?php echo CSS_DIR ?>/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
</style>
<script src="<?php echo JS_DIR ?>/jquery.js" type="text/javascript"></script>
<script src="<?php echo JS_DIR ?>/documentation.js" type="text/javascript"></script>
<link href="<?php echo JS_DIR ?>/documentation.css" type="text/css" rel="stylesheet" />
<!--// code-highlighting //-->
<script type="text/javaScript" src="/jquery/project/chili/jquery.chili-2.0.js"></script> 
<script type="text/javaScript" src="/jquery/project/chili-toolbar/jquery.chili-toolbar.pack.js"></script> 
<script type="text/javascript">try{ChiliBook.recipeFolder="/jquery/project/chili/"}catch(e){}</script>
<!--// plugin-specific resources //-->
<script src="<?php echo JS_DIR ?>/jquery.form.js" type="text/javascript" language="javascript"></script>
<script src="<?php echo JS_DIR ?>/jquery.MetaData.js" type="text/javascript" language="javascript"></script>
<script src="<?php echo JS_DIR ?>/jquery.MultiFile.js" type="text/javascript" language="javascript"></script>
<script src="<?php echo JS_DIR ?>/jquery.blockUI.js" type="text/javascript" language="javascript"></script>
<script type="text/javascript" src="<?php echo FCK_DIR ?>/fckeditor.js"></script>
<script type="text/javascript">
window.onload = function()
{
	// Automatically calculates the editor base path based on the _samples directory.
	// This is usefull only for these samples. A real application should use something like this:
	// oFCKeditor.BasePath = '/fckeditor/' ;	// '/fckeditor/' is the default value.
	var sBasePath = '<?php echo FCK_DIR ?>/' ;

	var oFCKeditor = new FCKeditor( 'detail' ) ;
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
  <div id="content" style="height:100%;"> <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'tpl-admin-menu.html', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
    <div id="ct_rt"  style="height:100%;">
    <form action="/index.php/Admin/goodseditdo" method="post" enctype="multipart/form-data" >
      <table class="table_list">
        <thead>
        <tr><td></td><td></td></tr>
       </thead>
        <tbody>
          <tr>
            <td>名称</td>
            <td><input class="tb350" type="text" name="name" value="<?php echo $this->_tpl_vars['goods']['name']; ?>
" /></td>
          </tr>
          <tr>
            <td>简述</td>
            <td><textarea class="tb350 hg100" name="summary"><?php echo $this->_tpl_vars['goods']['summary']; ?>
</textarea></td>
          </tr>          
          <tr>
            <td>详细信息</td>
            <td><textarea name="detail"><?php echo $this->_tpl_vars['goods']['detail']; ?>
</textarea></td>
          </tr>
          <tr>
            <td>类别</td>
            <td align="left"><?php echo $this->_tpl_vars['html_select']; ?>
</td>
          </tr>
          <tr>
            <td>图片</td>
            <td align="left"><img src="<?php echo GOODS_DIR ?>/<?php echo $this->_tpl_vars['goods']['image']; ?>
" alt="<?php echo $this->_tpl_vars['goods']['name']; ?>
">
            <input type="file" name="image[]" class="multi" accept="gif|jpg" maxlength="1" /></td>
          </tr>
          <tr>
            <td><input type="hidden" name="goodsid" value="<?php echo $this->_tpl_vars['goods']['goodsid']; ?>
" /></td>
            <td align="left"><input type="submit" class="btn" value="提交" /></td>
          </tr>
        </tbody>
      </table>
    </form>
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