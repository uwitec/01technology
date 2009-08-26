<?php /* Smarty version 2.6.18, created on 2009-07-30 09:20:34
         compiled from tpl-index.html */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'date_format', 'tpl-index.html', 28, false),array('modifier', 'strip_tags', 'tpl-index.html', 28, false),array('modifier', 'cntruncate', 'tpl-index.html', 28, false),)), $this); ?>
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "tpl-header.html", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
<body>
<div style="width:100%; height:100%; position:relative;">
<div id="body">
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "tpl-menu.html", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
<script type="text/javascript" src="<?php echo JS_DIR ?>/Groject.ImageSwitch.102.yui.js"></script>
<script type="text/javascript" src="<?php echo JS_DIR ?>/ads.js"></script>
 <div id="banner_index">
	<div id="ControlDiv"><a id="SlideShow">Play slide show</a></div>
	<div id="DisplayDiv">
		<img src="/Stuff/images/med-1.jpg" id="MainImage"/>
	</div>
	<div id="ThumbnailDiv">
		<img src="/Stuff/images/tn-1.jpg" class="TnImage"/>	
		<img src="/Stuff/images/tn-2.jpg" class="TnImage"/>
	</div>
 </div>
 <div id="index_content">
  <div id="notices">
   <ul>
    <li id="tab_news"  onmouseover="showPic('news', 'gg');">最新新闻</li>
    <li id="tab_notices" onmouseover="showPic('gg', 'news');">公告</li>
    <li id="tab_more"><a href="/index.php/News"><img src="<?php echo IMG_DIR ?>/more.gif" alt="more" /></a></li>
   </ul>
   <div id="news">
    <ol class="ct">
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
     <li><span><?php echo ((is_array($_tmp=$this->_tpl_vars['news'][$this->_sections['loop']['index']]['created'])) ? $this->_run_mod_handler('date_format', true, $_tmp, "%Y.%m.%d") : smarty_modifier_date_format($_tmp, "%Y.%m.%d")); ?>
</span><a href="/index.php/News/Desc/id/<?php echo $this->_tpl_vars['news'][$this->_sections['loop']['index']]['newsid']; ?>
"><?php echo ((is_array($_tmp=((is_array($_tmp=$this->_tpl_vars['news'][$this->_sections['loop']['index']]['title'])) ? $this->_run_mod_handler('strip_tags', true, $_tmp) : smarty_modifier_strip_tags($_tmp)))) ? $this->_run_mod_handler('cntruncate', true, $_tmp, 30, "...", true) : smarty_modifier_cntruncate($_tmp, 30, "...", true)); ?>
</a></li>
    <?php endfor; endif; ?>
    </ol>
   </div>
   <div id="gg" class="hide_list">
    <ol class="ct">
    <?php unset($this->_sections['loop']);
$this->_sections['loop']['name'] = 'loop';
$this->_sections['loop']['loop'] = is_array($_loop=$this->_tpl_vars['notices']) ? count($_loop) : max(0, (int)$_loop); unset($_loop);
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
     <li><span><?php echo ((is_array($_tmp=$this->_tpl_vars['notices'][$this->_sections['loop']['index']]['created'])) ? $this->_run_mod_handler('date_format', true, $_tmp, "%Y.%m.%d") : smarty_modifier_date_format($_tmp, "%Y.%m.%d")); ?>
</span><a href="/index.php/News/Desc/id/<?php echo $this->_tpl_vars['notices'][$this->_sections['loop']['index']]['newsid']; ?>
"><?php echo ((is_array($_tmp=((is_array($_tmp=$this->_tpl_vars['notices'][$this->_sections['loop']['index']]['title'])) ? $this->_run_mod_handler('strip_tags', true, $_tmp) : smarty_modifier_strip_tags($_tmp)))) ? $this->_run_mod_handler('cntruncate', true, $_tmp, 30, "...", true) : smarty_modifier_cntruncate($_tmp, 30, "...", true)); ?>
</a></li>
    <?php endfor; endif; ?>
    </ol>
   </div> 
  </div>
  <div id="products">
   <span><a style="cursor:pointer;" onmouseover="showPic('p1','p2');"><img src="<?php echo IMG_DIR ?>/product_arrow_lt.jpg" alt="" /></a>
   <a style="cursor:pointer;"  onmouseover="showPic('p2','p1');"><img src="<?php echo IMG_DIR ?>/product_arrow_rt.jpg" alt="" /></a></span>
   <h2><a href="/index.php/Products" >主导产品</a></h2>
   <div id="p1" class="show_list"><dl class="products_desc""><dt style="line-height:20px; text-align:center;"><?php echo $this->_tpl_vars['goods'][0]['name']; ?>
</dt><dd><?php echo $this->_tpl_vars['goods'][0]['summary']; ?>
</dd><dd style="text-align:right;"><a href="/index.php/Products/Desc/id/<?php echo $this->_tpl_vars['goods'][0]['goodsid']; ?>
">详情</a></dd></dl><img src="<?php echo GOODS_DIR ?>/<?php echo $this->_tpl_vars['goods'][0]['image']; ?>
" alt="<?php echo $this->_tpl_vars['goods'][0]['name']; ?>
"/></div>
   <div id="p2" class="hide_list"><dl class="products_desc"><dt style="line-height:20px; text-align:center;"><?php echo $this->_tpl_vars['goods'][1]['name']; ?>
</dt><dd><?php echo $this->_tpl_vars['goods'][1]['summary']; ?>
</dd><dd style="text-align:right;"><a href="/index.php/Products/Desc/id/<?php echo $this->_tpl_vars['goods'][1]['goodsid']; ?>
">详情</a></dd></dl><img src="<?php echo GOODS_DIR ?>/<?php echo $this->_tpl_vars['goods'][1]['image']; ?>
" alt="<?php echo $this->_tpl_vars['goods'][1]['name']; ?>
"/></div>   
  </div>
  <div id="contact">
   <h2><a href="/index.php/Contactus" >联系我们</a></h2>
   <div class="contact_ct">
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
</div>
</body>
</html>