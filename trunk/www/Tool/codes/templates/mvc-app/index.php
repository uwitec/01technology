<?php
/**
 * 典型 MVC 应用程序的入口文件
 */

// NO_LEGACY_FLEAPHP 为 true 时，指示不载入与 FleaPHP 1.0.6x 保持兼容的接口文件
define('NO_LEGACY_FLEAPHP', true);

/**
 * 当 DEPLOY_MODE 为 false 时，FleaPHP 使用调试模式运行，
 * 应用程序开发结束后，应该将 DEPLOY_MODE 改为 true。
 */
define('DEPLOY_MODE', false);

// 载入 FleaPHP，并作初始化
require('../FleaPHP/FLEA/FLEA.php');

// APP_DIR 常量定义为应用程序代码所在的父目录
define('APP_DIR', dirname(__FILE__) . DS . 'APP');

// 载入配置文件
FLEA::loadAppInf(APP_DIR . '/Config/APP_INF.php');
// TODO: 修改 DATABASE.php 内容为您所使用的数据库
FLEA::loadAppInf(APP_DIR . '/Config/DATABASE.php');


/**
 * 指定找不到控制器或动作方法时要调用的错误处理过程
 */
// FLEA::setAppInf('dispatcherFailedCallback', 'page_404');

// 导入应用程序对象搜索路径
FLEA::import(APP_DIR);

// 启动 MVC 模式，并运行应用程序
FLEA::runMVC();


/**
 * 当指定的控制器或者动作不存在时调用此方法
 *
 * 开发者可以在此方法中显示更友好的错误信息
 */
function page_404($controller, $action)
{
    echo "请求的控制器 \"{$controller}\" 没有找到，或者该控制器的动作 \"{$action}\" 没有定义。";
    exit;
}
