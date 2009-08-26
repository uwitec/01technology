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
 * 定义 FLEA_Com_RBAC_UsersManager 类，该类是 FLEA_Rbac_UsersManager 类的别名
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Deprecated
 * @version $Id: UsersManager.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

// {{{ includes
FLEA::loadClass('FLEA_Rbac_UsersManager');
// }}}

/**
 * 开发者应该直接使用 FLEA_Rbac_UsersManager 类
 *
 * @deprecated
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Com_RBAC_UsersManager extends FLEA_Rbac_UsersManager
{
}
