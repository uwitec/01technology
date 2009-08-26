<?php /* Smarty version 2.6.18, created on 2009-08-05 02:56:35
         compiled from library/page.lbi */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'url', 'library/page.lbi', 3, false),)), $this); ?>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <ul id="page">
   <li class="arrow_l"><a href="<?php echo $this->_plugins['function']['url'][0][0]->_pi_func_url(array('controller' => $this->_tpl_vars['url']['ctl'],'action' => $this->_tpl_vars['url']['act'],'page' => $this->_tpl_vars['goodsPager']['firstPageNumber']), $this);?>
" title='首页'></a></li>
   <?php unset($this->_sections['page']);
$this->_sections['page']['name'] = 'page';
$this->_sections['page']['loop'] = is_array($_loop=$this->_tpl_vars['Navbar']) ? count($_loop) : max(0, (int)$_loop); unset($_loop);
$this->_sections['page']['show'] = true;
$this->_sections['page']['max'] = $this->_sections['page']['loop'];
$this->_sections['page']['step'] = 1;
$this->_sections['page']['start'] = $this->_sections['page']['step'] > 0 ? 0 : $this->_sections['page']['loop']-1;
if ($this->_sections['page']['show']) {
    $this->_sections['page']['total'] = $this->_sections['page']['loop'];
    if ($this->_sections['page']['total'] == 0)
        $this->_sections['page']['show'] = false;
} else
    $this->_sections['page']['total'] = 0;
if ($this->_sections['page']['show']):

            for ($this->_sections['page']['index'] = $this->_sections['page']['start'], $this->_sections['page']['iteration'] = 1;
                 $this->_sections['page']['iteration'] <= $this->_sections['page']['total'];
                 $this->_sections['page']['index'] += $this->_sections['page']['step'], $this->_sections['page']['iteration']++):
$this->_sections['page']['rownum'] = $this->_sections['page']['iteration'];
$this->_sections['page']['index_prev'] = $this->_sections['page']['index'] - $this->_sections['page']['step'];
$this->_sections['page']['index_next'] = $this->_sections['page']['index'] + $this->_sections['page']['step'];
$this->_sections['page']['first']      = ($this->_sections['page']['iteration'] == 1);
$this->_sections['page']['last']       = ($this->_sections['page']['iteration'] == $this->_sections['page']['total']);
?>
    <?php if ($this->_tpl_vars['Navbar'][$this->_sections['page']['index']]['index'] == $this->_tpl_vars['Pager']['currentPage']): ?>
   <li><label>[<?php echo $this->_tpl_vars['Navbar'][$this->_sections['page']['index']]['number']; ?>
]</label></li><li class="page_point">.</li>
  <?php else: ?>
   <li><a href="<?php echo $this->_plugins['function']['url'][0][0]->_pi_func_url(array('controller' => $this->_tpl_vars['url']['ctl'],'action' => $this->_tpl_vars['url']['act'],'page' => $this->_tpl_vars['Navbar'][$this->_sections['page']['index']]['index']), $this);?>
"> <?php echo $this->_tpl_vars['Navbar'][$this->_sections['page']['index']]['number']; ?>
</a></li><li class="page_point">.</li>
  <?php endif; ?>
   <?php endfor; endif; ?>   
   <li class="arrow_r"><a href="<?php echo $this->_plugins['function']['url'][0][0]->_pi_func_url(array('controller' => $this->_tpl_vars['url']['ctl'],'action' => $this->_tpl_vars['url']['act'],'page' => $this->_tpl_vars['goodsPager']['lastPageNumber']), $this);?>
" title='末页'></a></li>
  </ul>