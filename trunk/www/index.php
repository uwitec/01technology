<?php
/////////////////////////////////////////////////////////////////////////////
// FleaPHP Framework
//
// Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
//
// 许可协议，请查看源代码中附带的 LICENSE.txt 文件，
// 或者访问 http://www.fleaphp.org/ 获得详细信息。
/////////////////////////////////////////////////////////////////////////////

/**
 * 尝试载入数据库配置文件，如果失败则显示错误页面
 */

$configFilename = './FLEA/DSN.php';
if (!is_readable($configFilename)) {
    header('Location: ./templates/tpl-error.html');
}

/**
 * Smarty 演示了如何在 FleaPHP 应用程序中使用 Smarty 模版引擎
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Example
 * @subpackage Smarty
 * @version $Id: index.php 972 2007-10-09 20:56:54Z qeeyuan $
 */



/**
 * 首先引入 FleaPHP 的库文件，并会做一些基本的处理
 */
define('NO_LEGACY_FLEAPHP', true);
require('./FLEA/FLEA.php');
require ('./config.inc.php');
/**
 * 指定数据库连接设置，TableDataGateway 会自动取出 dbDSN 设置来连接数据库。
 * FLEA::loadAppInf() 会用开发者指定的应用程序设置覆盖 FleaPHP 提供的默认设置。
 * 开发者可以使用 FLEA::getAppInf() 取出任意应用程序设置。
 */

/**
 * 要使用 Smarty，必须做两项准备工作
 *
 * 1、设置应用程序的 view 选项为 FLEA_View_Smarty；
 * 2、设置应用程序的 viewConfig 选项为数组，数组中必须包含
 *    smartyDir 选项，指示 Smarty 模版引擎源代码所在目录。
 *
 * 如果需要在构造 FLEA_View_Smarty 时就初始化 Smarty 模版引擎的设置，
 * 直接放置在 viewConfig 选项数组中即可。
 */

FLEA::setAppInf('urlMode', URL_PATHINFO);
FLEA::import(APP_DIR);
FLEA::runMVC();
