<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Exception_ExpectedClass */
?>

<h1>������ <strong><?php echo $ex->className; ?></strong> ʧ��</h1>

<div class="error">
<h2>��ϸ����ԭ��</h2>
�������������ҳ������Ϊ FleaPHP �޷�������Ҫ���� <strong><?php echo $ex->className; ?></strong>��<br />
ͨ����������Ϊ������û���ṩ������Ҫ���ඨ���ļ� <strong><?php echo h($ex->classFile); ?></strong>��<br />
�����ļ� <strong><?php echo h($ex->classFile); ?></strong> ��û���ṩ�� <strong><?php echo $ex->className; ?></strong> �Ķ��塣
</div>

<p></p>

<div class="tip">
<h2>�����</h2>
���� <strong><?php echo h($ex->classFile); ?></strong> �ļ��Ƿ���ڣ������ڸ��ļ��ж������� <strong><?php echo $ex->className; ?></strong>��
</div>

<p></p>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
