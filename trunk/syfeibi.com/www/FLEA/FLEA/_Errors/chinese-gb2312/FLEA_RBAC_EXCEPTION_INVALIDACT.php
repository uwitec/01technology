<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Rbac_Exception_InvalidACT */
?>

<h1>��Ч�ķ��ʿ��Ʊ�Access Control Table��</h1>

<div class="error">
<h2>��ϸ����ԭ��</h2>
�������������ҳ������Ϊ��ȡ��һ����Ч�ķ��ʿ��Ʊ�
</div>


<p>
<?php dump($ex->act, '���ʿ��Ʊ������'); ?>
</p>
<p>

<div class="tip">
<h2>�����</h2>
����� ACT �д��ڴ���ĵط���
</div>

<p></p>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
