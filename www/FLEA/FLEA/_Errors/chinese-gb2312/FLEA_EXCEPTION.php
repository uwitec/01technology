<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Exception */
?>

<h1><?php echo $ex->getMessage(); ?></h1>

<div class="error">
<h2>错误原因：</h2>
您看到这个错误页面是因为应用程序抛出了没有捕获的异常。
</div>

<p>
<strong>异常的详细内容：</strong>
<?php echo t($ex->__toString()); ?>
</p>

<p></p>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
