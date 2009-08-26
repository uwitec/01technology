<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Exception_ExpectedClass */
?>

<h1>加载类 <strong><?php echo $ex->className; ?></strong> 失败</h1>

<div class="error">
<h2>详细错误原因：</h2>
您看到这个错误页面是因为 FleaPHP 无法加载需要的类 <strong><?php echo $ex->className; ?></strong>。<br />
通常，这是因为开发者没有提供该类需要的类定义文件 <strong><?php echo h($ex->classFile); ?></strong>，<br />
或者文件 <strong><?php echo h($ex->classFile); ?></strong> 中没有提供类 <strong><?php echo $ex->className; ?></strong> 的定义。
</div>

<p></p>

<div class="tip">
<h2>解决：</h2>
请检查 <strong><?php echo h($ex->classFile); ?></strong> 文件是否存在，并且在该文件中定义了类 <strong><?php echo $ex->className; ?></strong>。
</div>

<p></p>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
