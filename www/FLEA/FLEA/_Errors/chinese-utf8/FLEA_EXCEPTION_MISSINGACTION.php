<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Exception_MissingAction */
?>

<h1>请求的控制器 <strong><?php echo $ex->controllerName; ?></strong> 的动作
<strong><?php echo $ex->actionName; ?></strong> 没有定义</h1>

<div class="error">
<h2>详细错误原因：</h2>
您请求访问控制器 <strong><?php echo $ex->controllerName; ?></strong>
的动作 <strong><?php echo $ex->actionName; ?></strong>。<br />
但该控制器的类定义文件
<p><strong><?php __error_filelink($ex->controllerClassFilename); ?></strong></p>
中没有定义动作 <strong><?php echo $ex->actionName; ?></strong>
对应的方法 <strong><?php echo $ex->actionMethod; ?></strong>。
</div>

<p>
<?php dump($ex->arguments, '调用参数'); ?>
</p>

<div class="tip">
<h2>解决：</h2>
请检查文件
<p><strong><?php __error_filelink($ex->controllerClassFilename); ?></strong></p>
中定义的 <strong><?php echo $ex->controllerClass; ?></strong> 类是否编写了
<strong><?php echo $ex->actionMethod; ?></strong> 方法。
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
