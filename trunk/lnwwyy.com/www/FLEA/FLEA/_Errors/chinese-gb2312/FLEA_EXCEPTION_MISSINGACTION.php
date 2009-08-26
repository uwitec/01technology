<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Exception_MissingAction */
?>

<h1>����Ŀ����� <strong><?php echo $ex->controllerName; ?></strong> �Ķ���
<strong><?php echo $ex->actionName; ?></strong> û�ж���</h1>

<div class="error">
<h2>��ϸ����ԭ��</h2>
��������ʿ����� <strong><?php echo $ex->controllerName; ?></strong>
�Ķ��� <strong><?php echo $ex->actionName; ?></strong>��<br />
���ÿ��������ඨ���ļ�
<p><strong><?php __error_filelink($ex->controllerClassFilename); ?></strong></p>
��û�ж��嶯�� <strong><?php echo $ex->actionName; ?></strong>
��Ӧ�ķ��� <strong><?php echo $ex->actionMethod; ?></strong>��
</div>

<p>
<?php dump($ex->arguments, '���ò���'); ?>
</p>

<div class="tip">
<h2>�����</h2>
�����ļ�
<p><strong><?php __error_filelink($ex->controllerClassFilename); ?></strong></p>
�ж���� <strong><?php echo $ex->controllerClass; ?></strong> ���Ƿ��д��
<strong><?php echo $ex->actionMethod; ?></strong> ������
</div>

<?php
$code = <<<EOT
<?php

// {$ex->controllerClassFilename}

class {$ex->controllerClass} extends FLEA_Controller_Action
{

    function {$ex->actionMethod}()
    {

    }
}
EOT;
__error_highlight_string($code);
?>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
