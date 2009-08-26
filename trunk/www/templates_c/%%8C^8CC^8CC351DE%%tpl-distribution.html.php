<?php /* Smarty version 2.6.18, created on 2009-07-29 08:48:30
         compiled from tpl-distribution.html */ ?>
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
 <div id="banner" style="background:#fff url('<?php echo IMG_DIR ?>/banner_dist.jpg') no-repeat;"></div>
 <div id="content">
  <div id="ct_lt">
	<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "tpl-menu-tree.html", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?> 
  </div>
  <div id="ct_rt">
   <div><h2 class="title">经销网络</h2><h3 class="e_title">
	   distribution</h3></div>
   <div class="nav"><a href="/">首页</a> &gt; 经销网络</div>
   <div class="sm_title"></div>   
   <div>
    <p  class="content distribution"></p>
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