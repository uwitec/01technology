<?php /* Smarty version 2.6.18, created on 2009-07-29 11:40:27
         compiled from tpl-admin-newslist.html */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'strip_tags', 'tpl-admin-newslist.html', 37, false),array('modifier', 'cntruncate', 'tpl-admin-newslist.html', 37, false),)), $this); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
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
      <table class="table_list" >
        <thead>
          <tr>
            <!--<td>&nbsp;</td>-->
            <td class="tb50">序号</td>
            <!--<td>排序</td>-->
            <td class="tb150">标题</td>
                        <td class="tb100">创建时间</td>
                        <td class="tb50">浏览数</td>
            <td class="tb50">作者</td>
            <td class="tb50">类型</td>
            <td class="tb100">操作</td>
          </tr>
        </thead>
        <tbody>
        <?php $i=1; ?>
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
        <tr>
          <!--<td><input type="checkbox" name="checkbox" value="newsid[<?php echo $this->_tpl_vars['news'][$this->_sections['loop']['index']]['newsid']; ?>
]"></td>-->
          <td><?php echo $i++ ?></td>
          <!--<td><input type="text" value="<?php echo $this->_tpl_vars['news'][$this->_sections['loop']['index']]['sort']; ?>
" size="4" /></td>-->
          <td><a href="/index.php/News/desc/id/<?php echo $this->_tpl_vars['news'][$this->_sections['loop']['index']]['newsid']; ?>
" target="_blank"><?php echo ((is_array($_tmp=((is_array($_tmp=$this->_tpl_vars['news'][$this->_sections['loop']['index']]['title'])) ? $this->_run_mod_handler('strip_tags', true, $_tmp) : smarty_modifier_strip_tags($_tmp)))) ? $this->_run_mod_handler('cntruncate', true, $_tmp, 10, "...", true) : smarty_modifier_cntruncate($_tmp, 10, "...", true)); ?>
</a></td>
                    <td><?php echo $this->_tpl_vars['news'][$this->_sections['loop']['index']]['created']; ?>
</td>
                    <td><?php echo $this->_tpl_vars['news'][$this->_sections['loop']['index']]['viewed']; ?>
</td>
          <td><?php echo $this->_tpl_vars['news'][$this->_sections['loop']['index']]['author']; ?>
</td>
          <td><?php if (( $this->_tpl_vars['news'][$this->_sections['loop']['index']]['type'] == 0 )): ?>新闻<?php elseif (( $this->_tpl_vars['news'][$this->_sections['loop']['index']]['type'] == 1 )): ?><b>公告</b><?php endif; ?></td>
          <td><a href="/index.php/Admin/Newsdel/id/<?php echo $this->_tpl_vars['news'][$this->_sections['loop']['index']]['newsid']; ?>
">删除</a> <a href="/index.php/Admin/Newsedit/id/<?php echo $this->_tpl_vars['news'][$this->_sections['loop']['index']]['newsid']; ?>
">编辑</a></td>
        </tr>
        <?php endfor; endif; ?>
        </tbody>
      </table>
      <div align="left" style="width:95%">
        <!--<input type="submit" value="删除" name="submit">-->
        <input type="button" class="btn" onclick="javascript:window.location.href='/index.php/Admin/Newsedit'" value="添加新闻" />
	  <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'tpl-page.html', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
      </div>
    </div>
  </div>
  <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'tpl-admin-footer.html', 'smarty_include_vars' => array()));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?> </div>
</body>
</html>