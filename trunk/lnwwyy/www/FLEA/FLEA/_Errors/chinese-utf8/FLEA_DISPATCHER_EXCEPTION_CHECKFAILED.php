<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Dispatcher_Exception_CheckFailed */
?>

<h1>拒绝访问控制器 <strong><?php echo $ex->controllerName; ?></strong> 的 <strong><?php echo $ex->actionName; ?></strong> 动作</h1>

<div class="error">
<h2>详细错误原因：</h2>
您请求访问控制器 <strong><?php echo $ex->controllerName; ?></strong>
的动作 <strong><?php echo $ex->actionName; ?></strong>。<br />
但您没有访问该控制器或控制器动作的所要求的权限。
</div>

<p>
<?php dump(implode(', ', (array)$ex->roles), '当前访问用户具有的角色信息'); ?>
<?php dump($ex->act, "控制器 {$ex->controllerName} 的访问控制表 (ACT)"); ?>
</p>

<div class="tip">
<h2>解决：</h2>
请检查您是否已经登录应用程序，并且使用的用户账户具有应用程序访问规则要求的角色信息。
</div>

<p></p>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
