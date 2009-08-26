<?php /* Smarty version 2.6.18, created on 2009-07-28 21:52:44
         compiled from tpl-products-desc.html */ ?>
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "tpl-header.html", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
<body>
<div id="body">
 <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "tpl-menu.html", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
 <div id="banner" style="background:#fff url('<?php echo IMG_DIR ?>/banner_pro.jpg') no-repeat;"></div>
 <div id="content">
  <div id="ct_lt">
	<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "tpl-menu-tree.html", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?> 
  </div>
  <div id="ct_rt">
   <div><h2 class="title">产品世界</h2><h3 class="e_title">products</h3></div>
   <div class="nav"><a href="/">首页</a> &gt; <a href="/index.php/Products">产品世界</a> &gt; <?php echo $this->_tpl_vars['good']['name']; ?>
</div>
   <div class="sm_title"></div>   
   <div class="content" style="background:#fff url('<?php echo GOODS_DIR ?>/<?php echo $this->_tpl_vars['good']['image']; ?>
') no-repeat; background-position:right top;">
	<h2><?php echo $this->_tpl_vars['good']['name']; ?>
</h2>
	<p><?php echo $this->_tpl_vars['good']['detail']; ?>
</p>
   </div>
  </div>
 </div>
 <div style="clear:both"></div>
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "tpl-footer.html", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
</div>
</body>
</html>