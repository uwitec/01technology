<?php /* Smarty version 2.6.18, created on 2009-07-28 21:53:43
         compiled from tpl-news.html */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'date_format', 'tpl-news.html', 17, false),array('modifier', 'strip_tags', 'tpl-news.html', 17, false),array('modifier', 'cntruncate', 'tpl-news.html', 17, false),)), $this); ?>
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
 <div id="banner" style="background:#fff url('<?php echo IMG_DIR ?>/banner_news.jpg') no-repeat;"></div>
 <div id="content">
  <div id="ct_lt">
	<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "tpl-menu-tree.html", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?> 
  </div>
  <div id="ct_rt">
   <div><h2 class="title">新闻中心</h2><h3 class="e_title">news</h3></div>
   <div class="nav"><a href="/">首页</a> &gt; 新闻中心</div>
   <div class="sm_title"></div>   
   <div class="content">
    <dl>
     <?php unset($this->_sections['loop']);
$this->_sections['loop']['name'] = 'loop';
$this->_sections['loop']['loop'] = is_array($_loop=$this->_tpl_vars['news']) ? count($_loop) : max(0, (int)$_loop); unset($_loop);
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
     <dt><span><?php echo ((is_array($_tmp=$this->_tpl_vars['news'][$this->_sections['loop']['index']]['created'])) ? $this->_run_mod_handler('date_format', true, $_tmp, "%Y.%m.%d") : smarty_modifier_date_format($_tmp, "%Y.%m.%d")); ?>
</span><a href="/index.php/News/Desc/id/<?php echo $this->_tpl_vars['news'][$this->_sections['loop']['index']]['newsid']; ?>
"><?php echo ((is_array($_tmp=((is_array($_tmp=$this->_tpl_vars['news'][$this->_sections['loop']['index']]['title'])) ? $this->_run_mod_handler('strip_tags', true, $_tmp) : smarty_modifier_strip_tags($_tmp)))) ? $this->_run_mod_handler('cntruncate', true, $_tmp, 80, "...", true) : smarty_modifier_cntruncate($_tmp, 80, "...", true)); ?>
</a></dt>
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