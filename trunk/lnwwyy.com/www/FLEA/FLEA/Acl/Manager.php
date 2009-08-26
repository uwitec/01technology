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
 * 定义 FLEA_Acl_Manager 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Manager.php 1060 2008-05-04 05:02:59Z qeeyuan $
 */

/**
 * FLEA_Acl_Manager 提供 ACL 数据的全面管理功能
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Acl_Manager
{
    /**
     * 所有使用到的表数据对象类名称
     *
     * @var array
     */
    var $_tableClass = array(
        'users' =>                  'FLEA_Acl_Table_Users',
        'roles' =>                  'FLEA_Acl_Table_Roles',
        'userGroups' =>             'FLEA_Acl_Table_UserGroups',
        'permissions' =>            'FLEA_Acl_Table_Permissions',
        'userGroupsHasRoles' =>     'FLEA_Acl_Table_UserGroupsHasRoles',
        'userGroupsHasPermissions' => 'FLEA_Acl_Table_UserGroupsHasPermissions',
        'userHasRoles' =>           'FLEA_Acl_Table_UserHasRoles',
        'userHasPermissions' =>     'FLEA_Acl_Table_UserHasPermissions',
    );

    function FLEA_Acl_Manager($tableClass = array())
    {
        $this->_tableClass = array_merge($this->_tableClass, (array)$tableClass);
    }

    /**
     * 获取指定用户，及其权限信息
     *
     * @param array $conditions
     */
    function getUserWithPermissions($conditions)
    {
        $tableUsers =& FLEA::getSingleton($this->_tableClass['users']);
        /* @var $tableUsers FLEA_Acl_Table_Users */
        $user = $tableUsers->find($conditions);
        if (empty($user)) { return false; }

        // 取得用户所在用户组的层次数据
        $tableUserGroups =& FLEA::getSingleton($this->_tableClass['userGroups']);
        /* @var $tableUserGroups FLEA_Acl_Table_UserGroups */
        $rowset = $tableUserGroups->getPath($user['group']);

        // 找出用户组的单一路径
        FLEA::loadHelper('array');
        $ret = array_to_tree($rowset, 'user_group_id', 'parent_id', 'subgroups', true);
        $tree =& $ret['tree'];
        $refs =& $ret['refs'];
        $groupid = $user['user_group_id'];
        $path = array();
        while (isset($refs[$groupid])) {
            array_unshift($path, $refs[$groupid]);
            $groupid = $refs[$groupid]['parent_id'];
        }

        // 整理角色信息
        $userRoles = array();

        foreach ($path as $group) {
            $roles = $group['roles'];
            foreach ($roles as $role) {
                $roleid = $role['role_id'];
                if ($role['_join_is_include']) {
                    $userRoles[$roleid] = array('role_id' => $roleid, 'name' => $role['name']);
                } else {
                    unset($userRoles[$roleid]);
                }
            }
        }

        foreach ((array)$user['roles'] as $role) {
            $roleid = $role['role_id'];
            if ($role['_join_is_include']) {
                $userRoles[$roleid] = array('role_id' => $roleid, 'name' => $role['name']);
            } else {
                unset($userRoles[$roleid]);
            }
        }

        // 整理权限信息
        $user['roles'] = $userRoles;
        return $user;
    }
}
