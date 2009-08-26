<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Exception_CacheDisabled */
$cacheDir = realpath($ex->cacheDir);
?>

<h1>FleaPHP 内部缓存无法使用</h1>

<div class="error">
<h2>详细错误原因：</h2>
您看到这个错误页面是因为 FleaPHP 内部缓存功能无法使用。<br />
通常，这是因为开发者没有指定应用程序设置 internalCacheDir 指向一个保存缓存数据的目录，<br />
或者指定的目录无法写入文件。
</div>

<p>
<strong>指定的缓存目录：</strong>
<?php if ($ex->cacheDir == ''): ?>
没有指定缓存目录
<?php elseif ($cacheDir == ''): ?>
不存在的缓存目录（<?php echo h($ex->cacheDir); ?>）
<?php else: ?>
<?php echo h($cacheDir); ?>
<?php endif; ?>
</p>

<div class="tip">
<h2>解决：</h2>
请检查您是否已经指定了缓存目录，或者缓存目录是否存在，并且可以创建文件。
</div>

<p></p>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
