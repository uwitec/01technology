<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Exception_MissingController */
?>

<h1>请求的控制器 <strong><?php echo $ex->controllerName; ?></strong> 没有定义</h1>

<div class="error">
<h2>详细错误原因：</h2>
您请求访问控制器 <strong><?php echo $ex->controllerName; ?></strong>
的动作 <strong><?php echo $ex->actionName; ?></strong>。<br />
但控制器 <strong><?php echo $ex->controllerName; ?></strong>
对应的类 <strong><?php echo $ex->controllerClass; ?></strong>
<?php if ($ex->controllerClassFilename): ?>
在文件
<p><strong><?php __error_filelink($ex->controllerClassFilename); ?></strong></p>
中没有定义。
<?php else: ?>
没有定义。
<?php endif; ?>
</div>

<p>
<?php dump($ex->arguments, '调用参数'); ?>
</p>

<div class="tip">
<h2>解决：</h2>
<?php
if ($ex->controllerClassFilename): 
$controllerClassFilename = $ex->controllerClassFilename;
?>
请检查文件
<p><strong><?php __error_filelink($ex->controllerClassFilename); ?></strong></p>
中是否有 <strong><?php echo $ex->controllerClass; ?></strong> 类的定义。
<?php
else:
$controllerClassFilename = FLEA::getFilePath($ex->controllerClass . '.php', true);
?>
请检查是否创建了 <strong><?php echo $ex->controllerClass; ?></strong> 类的定义文件：
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
