<?php /* Smarty version 2.6.18, created on 2009-07-28 21:52:03
         compiled from tpl-feedback.html */ ?>
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
 <div id="banner" style=" background:#fff url('<?php echo IMG_DIR ?>/banner_fb.jpg') no-repeat;"></div>
 <div id="content">
  <div id="ct_lt">
	<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "tpl-menu-tree.html", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?> 
  </div>
  <div id="ct_rt">
   <div><h2 class="title">信息反馈</h2><h3 class="e_title">feedback</h3></div>
   <div class="nav"><a href="/">首页</a> &gt; 信息反馈</div>
   <div class="sm_title"></div>   
   <div class="content">
   <form action="/index.php/Feedback/Post" method="post">
    <ul>
     <li><label>主题名称 </label><input name="title" type="text" class="text" /></li>    
     <li><label style="line-height:150px; vertical-align:top;">反馈内容 </label><textarea name="content" class="textarea"></textarea></li>
     <li><label>客户名称 </label><input name="user" type="text" class="text" /></li>
     <li><label>联系电话 </label><input name="tel" type="text" class="text" /></li>
     <li><label>电子邮箱 </label><input name="email" type="text" class="text" /></li>
     <li style="text-align:right;"><input name="" type="submit" value="提交"  class="button"/> <input name="" type="reset" value="重填" class="button"/></li>      
    </ul>
    </form>    
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