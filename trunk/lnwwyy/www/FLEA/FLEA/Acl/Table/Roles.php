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
 * 定义 FLEA_Acl_Table_Roles 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Roles.php 1060 2008-05-04 05:02:59Z qeeyuan $
 */

// {{{ includes
FLEA::loadClass('FLEA_Db_TableDataGateway');
// }}}

/**
 * FLEA_Acl_Table_Roles 提供了角色数据的存储服务
 *
 * @package Core
 */
class FLEA_Acl_Table_Roles extends FLEA_Db_TableDataGateway
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
     * 一个角色对应多个权限，一个权限可以指派给多个角色
     *
     * @var array
     */
    var $manyToMany = array(
        array(
            'tableClass' => 'FLEA_Acl_Table_Permissions',
            'foreignKey' => 'role_id',
            'assocForeignKey' => 'permission_id',
            'joinTable' => 'roles_has_permissions',
            'mappingName' => 'permissions',
        ),
    );

}
