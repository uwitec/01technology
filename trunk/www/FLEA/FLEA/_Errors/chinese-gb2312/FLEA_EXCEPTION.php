<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Exception */
?>

<h1><?php echo $ex->getMessage(); ?></h1>

<div class="error">
<h2>����ԭ��</h2>
�������������ҳ������ΪӦ�ó����׳���û�в�����쳣��
</div>

<p>
<strong>�쳣����ϸ���ݣ�</strong>
<?php echo t($ex->__toString()); ?>
</p>

<p></p>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
