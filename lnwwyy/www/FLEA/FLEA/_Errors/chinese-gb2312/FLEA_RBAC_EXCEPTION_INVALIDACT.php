<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Rbac_Exception_InvalidACT */
?>

<h1>无效的访问控制表（Access Control Table）</h1>

<div class="error">
<h2>详细错误原因：</h2>
您看到这个错误页面是因为读取了一个无效的访问控制表。
</div>


<p>
<?php dump($ex->act, '访问控制表的内容'); ?>
</p>
<p>

<div class="tip">
<h2>解决：</h2>
请检查该 ACT 中存在错误的地方。
</div>

<p></p>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
