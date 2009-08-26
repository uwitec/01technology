<?php /* Smarty version 2.6.18, created on 2009-08-09 14:02:19
         compiled from tpl-albums.dwt */ ?>
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
  <div class="path_url">当前位置:<a href="/">首页</a> &gt; 新娘相册</div>  
  <div class="page_title">
  <h2>新娘相册</h2>
  <h3>WEDDING ALBUM</h3></div>
  <div class="show_album_img">
   <div class="ding_l"></div>
   <div class="ding_r"></div>
   <a id="show_main_img"><img src="<?php echo GOODS_DIR ?>/<?php echo $this->_tpl_vars['show']['image']; ?>
" alt="<?php echo $this->_tpl_vars['show']['name']; ?>
" /></a>
  </div>
  <div class="show_album_list">
   <h3>照片名称</h3>
   <p>说明</p>
   <ul class="show_album_menu">
    <li id="show_all"><img src="<?php echo IMG_DIR ?>/album_button_all_sel.png" alt="全部作品展示"/></li>
    <li id="show_hz"><a href="/index.php/Albums/Hz"><img src="<?php echo IMG_DIR ?>/album_button_hz_no.png" alt="化妆作品展示" /></a></li>
    <li id="show_hs"><a href="/index.php/Albums/Hs"><img src="<?php echo IMG_DIR ?>/album_button_hs_no.png" alt="婚纱礼服展示" /></a></li>
   </ul>
    <ul id="pic_all" class="show_album_pics lightbox">
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
     <li><a href="<?php echo GOODS_DIR ?>/<?php echo $this->_tpl_vars['goods'][$this->_sections['loop']['index']]['image']; ?>
"><img id="img<?php echo $this->_tpl_vars['goods'][$this->_sections['loop']['index']]['goodsid']; ?>
" onmousemove="disp_main_pic('img<?php echo $this->_tpl_vars['goods'][$this->_sections['loop']['index']]['goodsid']; ?>
');" src="<?php echo GOODS_DIR ?>/<?php echo $this->_tpl_vars['goods'][$this->_sections['loop']['index']]['thums']; ?>
" rel="<?php echo GOODS_DIR ?>/<?php echo $this->_tpl_vars['goods'][$this->_sections['loop']['index']]['image']; ?>
" /></a></li>
     <?php endfor; endif; ?>        
    </ul>      
   <div style="margin:0px auto; clear:both;">
   <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "library/page.lbi", 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
   </div>
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