<?php /* Smarty version 2.6.18, created on 2009-07-28 21:51:57
         compiled from tpl-products.html */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'truncate', 'tpl-products.html', 22, false),array('modifier', 'cntruncate', 'tpl-products.html', 22, false),)), $this); ?>
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "tpl-header.html", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
<body>
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
   <div class="nav"><a href="/">首页</a> &gt; 产品世界</div>
   <div class="sm_title"></div>   
   <div class="products">
    <dl>
     <?php unset($this->_sections['loop']);
$this->_sections['loop']['name'] = 'loop';
$this->_sections['loop']['loop'] = is_array($_loop=$this->_tpl_vars['goods']) ? count($_loop) : max(0, (int)$_loop); unset($_loop);
$this->_sections['loop']['show'] = true;
$this->_sections['loop']['max'] = $this->_sections['loop']['loop'];
$this->_sections['loop']['step'] = 1;
$this->_sections['loop']['start'] = $this->_sections['loop']['step'] > 0 ? 0 : $this->_sections['loop']['loop']-1;
if ($this->_sections['loop']['show']) {
    $this->_sections['loop']['total'] = $this->_sections['loop']['loop'];
    if ($this->_sections['loop']['total'] == 0)
        $this->_sections['loop']['show'] = false;
} else
    $this->_sections['loop']['total'] = 0;
if ($this->_sections['loop']['show']):

            for ($this->_sections['loop']['index'] = $this->_sections['loop']['start'], $this->_sections['loop']['iteration'] = 1;
                 $this->_sections['loop']['iteration'] <= $this->_sections['loop']['total'];
                 $this->_sections['loop']['index'] += $this->_sections['loop']['step'], $this->_sections['loop']['iteration']++):
$this->_sections['loop']['rownum'] = $this->_sections['loop']['iteration'];
$this->_sections['loop']['index_prev'] = $this->_sections['loop']['index'] - $this->_sections['loop']['step'];
$this->_sections['loop']['index_next'] = $this->_sections['loop']['index'] + $this->_sections['loop']['step'];
$this->_sections['loop']['first']      = ($this->_sections['loop']['iteration'] == 1);
$this->_sections['loop']['last']       = ($this->_sections['loop']['iteration'] == $this->_sections['loop']['total']);
?>    
     <dt class="clear-float"></dt>    
     <dd class="lt"><img src="<?php echo GOODS_DIR ?>/<?php echo $this->_tpl_vars['goods'][$this->_sections['loop']['index']]['image']; ?>
" alt="<?php echo $this->_tpl_vars['goods'][$this->_sections['loop']['index']]['name']; ?>
" /></dd>
     <dd class="products_dd">
      <h2 style="text-indent:20px; font-size:18px;"><?php echo $this->_tpl_vars['goods'][$this->_sections['loop']['index']]['name']; ?>
</h2>
      <p><?php echo ((is_array($_tmp=((is_array($_tmp=$this->_tpl_vars['goods'][$this->_sections['loop']['index']]['detail'])) ? $this->_run_mod_handler('truncate', true, $_tmp, 300) : smarty_modifier_truncate($_tmp, 300)))) ? $this->_run_mod_handler('cntruncate', true, $_tmp, 300, "...", true) : smarty_modifier_cntruncate($_tmp, 300, "...", true)); ?>
</p>
      <a href="/index.php/Products/Desc/id/<?php echo $this->_tpl_vars['goods'][$this->_sections['loop']['index']]['goodsid']; ?>
">详细信息</a>					
	 </dd>
     <!-- <dt style="clear:both;margin:20px 20px; border-bottom: 1px #E9E9E9 solid;"></dt> -->
     <?php endfor; endif; ?>
    </dl>
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