<?php /* Smarty version 2.6.18, created on 2009-07-29 11:41:03
         compiled from tpl-admin-msglist.html */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'strip_tags', 'tpl-admin-msglist.html', 35, false),array('modifier', 'cntruncate', 'tpl-admin-msglist.html', 35, false),)), $this); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>后台管理界面</title>
<link href="<?php echo CSS_DIR ?>/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
</style>
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
    <div id="ct_rt">
      <table class="table_list">
        <thead>
          <tr>
            <!--<td>&nbsp;</td>-->
	        <td class="tb50">序号</td>
	        <td class="tb250">标题</td>
	        	        <td class="tb100">创建时间</td>
	        <td class="tb100">客户名称</td>
	        <td class="tb100">E-mail</td>
	        <td class="tb100">电话</td>
	        <td class="tb50">是否已读</td>
	        <td class="tb50">操作</td>
	      </tr>
    	</thead>
        <tbody>
        <?php $i=1; ?>
	    <?php unset($this->_sections['loop']);
$this->_sections['loop']['name'] = 'loop';
$this->_sections['loop']['loop'] = is_array($_loop=$this->_tpl_vars['msgs']) ? count($_loop) : max(0, (int)$_loop); unset($_loop);
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
	    <tr>
	      <!--<td><input type="checkbox" name="checkbox" value="newsid[<?php echo $this->_tpl_vars['msgs'][$this->_sections['loop']['index']]['msgid']; ?>
]" /></td>-->
	      <td><?php echo $i++ ?></td>
	      <td><a href="/index.php/Admin/Msgshow/id/<?php echo $this->_tpl_vars['msgs'][$this->_sections['loop']['index']]['msgid']; ?>
"><?php echo ((is_array($_tmp=((is_array($_tmp=$this->_tpl_vars['msgs'][$this->_sections['loop']['index']]['title'])) ? $this->_run_mod_handler('strip_tags', true, $_tmp) : smarty_modifier_strip_tags($_tmp)))) ? $this->_run_mod_handler('cntruncate', true, $_tmp, 10, "...", true) : smarty_modifier_cntruncate($_tmp, 10, "...", true)); ?>
</a></td>
	      	      <td><?php echo $this->_tpl_vars['msgs'][$this->_sections['loop']['index']]['created']; ?>
</td>
	      <td><?php echo $this->_tpl_vars['msgs'][$this->_sections['loop']['index']]['user']; ?>
</td>
	      <td><?php echo $this->_tpl_vars['msgs'][$this->_sections['loop']['index']]['email']; ?>
</td>
	      <td><?php echo $this->_tpl_vars['msgs'][$this->_sections['loop']['index']]['tel']; ?>
</td>
	      <td><?php if (( $this->_tpl_vars['msgs'][$this->_sections['loop']['index']]['viewed'] == 0 )): ?><font color="#FF0000">未读</font><?php else: ?>已读<?php endif; ?></td>
	      <td><a href="/index.php/Admin/Msgdel/id/<?php echo $this->_tpl_vars['msgs'][$this->_sections['loop']['index']]['msgid']; ?>
">删除</a></td>
	    </tr>
	    <?php endfor; endif; ?>
	    </tbody>
	  </table>
	</div>
  <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'tpl-page.html', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
</div>
<?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'tpl-admin-footer.html', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?> </div>
</body>
</html>