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
 * 定义 FLEA_Rbac_RolesManager 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: RolesManager.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

// {{{ includes
FLEA::loadClass('FLEA_Db_TableDataGateway');
// }}}

/**
 * FLEA_Rbac_RolesManager 派生自 FLEA_Db_TableDataGateway，
 * 用于访问保存角色信息的数据表
 *
 * 如果数据表的名字不同，应该从 FLEA_Rbac_RolesManager
 * 派生类并使用自定义的数据表名字、主键字段名等。
 *
 * @package Core
 */
class FLEA_Rbac_RolesManager extends FLEA_Db_TableDataGateway
{
    /**
     * 主键字段名
     *
     * @var string
     */
    var $primaryKey = 'role_id';

    /**
     * 数据表名字
     *
     * @var string
     */
    var $tableName = 'roles';

    /**
     * 角色名字段
     *
     * @var string
     */
    var $rolesNameField = 'rolename';

    /**
     * 构造函数
     *
     * @param array $params
     *
     * @return FLEA_Rbac_RolesManager
     */
    function FLEA_Rbac_RolesManager($params = null)
    {
        parent::FLEA_Db_TableDataGateway($params);
    }
}
