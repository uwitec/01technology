<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Dispatcher_Exception_CheckFailed */
?>

<h1>�ܾ����ʿ����� <strong><?php echo $ex->controllerName; ?></strong> �� <strong><?php echo $ex->actionName; ?></strong> ����</h1>

<div class="error">
<h2>��ϸ����ԭ��</h2>
��������ʿ����� <strong><?php echo $ex->controllerName; ?></strong>
�Ķ��� <strong><?php echo $ex->actionName; ?></strong>��<br />
����û�з��ʸÿ��������������������Ҫ���Ȩ�ޡ�
</div>

<p>
<?php dump(implode(', ', (array)$ex->roles), '��ǰ�����û����еĽ�ɫ��Ϣ'); ?>
<?php dump($ex->act, "������ {$ex->controllerName} �ķ��ʿ��Ʊ� (ACT)"); ?>
</p>

<div class="tip">
<h2>�����</h2>
�������Ƿ��Ѿ���¼Ӧ�ó��򣬲���ʹ�õ��û��˻�����Ӧ�ó�����ʹ���Ҫ��Ľ�ɫ��Ϣ��
</div>

<p></p>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
