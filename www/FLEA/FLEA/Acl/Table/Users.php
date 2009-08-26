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
 * 定义 FLEA_Acl_Table_Users 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Users.php 1060 2008-05-04 05:02:59Z qeeyuan $
 */

// {{{ includes
FLEA::loadClass('FLEA_Rbac_UsersManager');
// }}}

/**
 * FLEA_Acl_Table_Users 提供用户数据的存储服务
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Acl_Table_Users extends FLEA_Rbac_UsersManager
{
    var $belongsTo = array(
        array(
            'tableClass' => 'FLEA_Acl_Table_UserGroups',
            'foreignKey' => 'user_group_id',
            'mappingName' => 'group',
        ),
    );

    var $manyToMany = array(
        array(
            'tableClass' => 'FLEA_Acl_Table_Roles',
            'foreignKey' => 'user_id',
            'assocForeignKey' => 'role_id',
            'joinTableClass' => 'FLEA_Acl_Table_UsersHasRoles',
            'mappingName' => 'roles',
        ),
        array(
            'tableClass' => 'FLEA_Acl_Table_Permissions',
            'foreignKey' => 'user_id',
            'assocForeignKey' => 'permission_id',
            'joinTableClass' => 'FLEA_Acl_Table_UsersHasPermissions',
            'mappingName' => 'permissions',
        ),
    );
}
