<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Exception_MissingController */
?>

<h1>����Ŀ����� <strong><?php echo $ex->controllerName; ?></strong> û�ж���</h1>

<div class="error">
<h2>��ϸ����ԭ��</h2>
��������ʿ����� <strong><?php echo $ex->controllerName; ?></strong>
�Ķ��� <strong><?php echo $ex->actionName; ?></strong>��<br />
�������� <strong><?php echo $ex->controllerName; ?></strong>
��Ӧ���� <strong><?php echo $ex->controllerClass; ?></strong>
<?php if ($ex->controllerClassFilename): ?>
���ļ�
<p><strong><?php __error_filelink($ex->controllerClassFilename); ?></strong></p>
��û�ж��塣
<?php else: ?>
û�ж��塣
<?php endif; ?>
</div>

<p>
<?php dump($ex->arguments, '���ò���'); ?>
</p>

<div class="tip">
<h2>�����</h2>
<?php
if ($ex->controllerClassFilename): 
$controllerClassFilename = $ex->controllerClassFilename;
?>
�����ļ�
<p><strong><?php __error_filelink($ex->controllerClassFilename); ?></strong></p>
���Ƿ��� <strong><?php echo $ex->controllerClass; ?></strong> ��Ķ��塣
<?php
else:
$controllerClassFilename = FLEA::getFilePath($ex->controllerClass . '.php', true);
?>
�����Ƿ񴴽��� <strong><?php echo $ex->controllerClass; ?></strong> ��Ķ����ļ���
<p><strong><?php __error_filelink($controllerClassFilename); ?></strong></p>
<?php endif; ?>
</div>

<?php
$code = <<<EOT
<?php

// {$controllerClassFilename}

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
