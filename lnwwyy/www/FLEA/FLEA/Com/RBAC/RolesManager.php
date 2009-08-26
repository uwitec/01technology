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
 * 定义 FLEA_Com_RBAC_RolesManager 类，该类仅仅是 FLEA_Rbac_RolesManager 的别名
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Deprecated
 * @version $Id: RolesManager.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

// {{{ includes
FLEA::loadClass('FLEA_Rbac_RolesManager');
// }}}

/**
 * 开发者应该直接使用 FLEA_Rbac_RolesManager 类
 *
 * @deprecated
 * @package Core
 */
class FLEA_Com_RBAC_RolesManager extends FLEA_Rbac_RolesManager
{
}
