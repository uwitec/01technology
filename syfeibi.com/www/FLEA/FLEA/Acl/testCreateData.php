<?php

require('FLEA.php');

$dbDSN = array(
    'driver'    => 'mysqlt',
    'host'      => 'localhost',
    'login'     => 'root',
    'password'  => '',
    'database'  => 'test'
);

FLEA::setAppInf('dbDSN', $dbDSN);
FLEA::setAppInf('internalCacheDir', 'D:/temp');

$dbo =& FLEA::getDBO();
$dbo->startTrans();

/**
 * 建立全部需要的权限
 */
$tablePermissions =& FLEA::getSingleton('FLEA_Acl_Table_Permissions');
/* @var $tablePermissions FLEA_Acl_Table_Permissions */
$permissions = array(
    array('name' => '/Project/Create'),
    array('name' => '/Project/View'),
    array('name' => '/Project/Edit'),
    array('name' => '/Project/Delete'),
    array('name' => '/Bug/Create'),
    array('name' => '/Bug/View'),
    array('name' => '/Bug/Edit'),
    array('name' => '/Bug/AddComment'),
    array('name' => '/Bug/SetFixed'),
    array('name' => '/Bug/SetClosed'),
    array('name' => '/Bug/Delete'),
);
$tablePermissions->createRowset($permissions);

$permissions = $tablePermissions->findAll();
FLEA::loadHelper('array');
$permissions = array_to_hashmap($permissions, 'name');

/**
 * 建立角色，并将权限绑定到角色上
 */
$tableRoles =& FLEA::getSingleton('FLEA_Acl_Table_Roles');
/* @var $tableRoles FLEA_Acl_Table_Roles */
$role = array(
    'name' => 'ProjectManager',
    'permissions' => array(
        $permissions['/Project/Create'],
        $permissions['/Project/View'],
        $permissions['/Project/Edit'],
        $permissions['/Project/Delete'],
        $permissions['/Bug/Delete'],
    ),
);
$tableRoles->create($role);

$role = array(
    'name' => 'Developer',
    'permissions' => array(
        $permissions['/Project/View'],
        $permissions['/Bug/View'],
        $permissions['/Bug/AddComment'],
        $permissions['/Bug/SetFixed'],
        $permissions['/Bug/Delete'],
    ),
);
$tableRoles->create($role);

$role = array(
    'name' => 'Tester',
    'permissions' => array(
        $permissions['/Project/Create'],
        $permissions['/Bug/Create'],
        $permissions['/Bug/Edit'],
        $permissions['/Bug/View'],
        $permissions['/Bug/AddComment'],
        $permissions['/Bug/SetClosed'],
    ),
);
$tableRoles->create($role);

/**
 * 读取所有角色信息，并以角色名为索引
 */
$roles = $tableRoles->findAll();
$roles = array_to_hashmap($roles, 'name');

/**
 * 创建用户组层次，并指定角色
 *
 * 开发组
 *   |
 *   +----- QeePHP Team
 *   |
 *   +----- PHPChina Team
 *   |
 *   \----- 测试组
 */
$tableUserGroups =& FLEA::getSingleton('FLEA_Acl_Table_UserGroups');
/* @var $tableUserGroups FLEA_Acl_Table_UserGroups */
$group = array(
    'name' => '开发组',
    'roles' => array(
        $roles['Developer'],
    )
);
$tableUserGroups->create($group);
$parent = $tableUserGroups->find(array('name' => '开发组'));

$group = array(
    'name' => 'QeePHP Team',
    'parent_id' => $parent['user_group_id'],
    'roles' => array(
        $roles['Developer'],
    )
);
$tableUserGroups->create($group);

$group = array(
    'name' => 'PHPChina Team',
    'parent_id' => $parent['user_group_id'],
    'roles' => array(
        $roles['Developer'],
    )
);
$tableUserGroups->create($group);

$group = array(
    'name' => '测试组',
    'parent_id' => $parent['user_group_id'],
    'roles' => array(
        $roles['Tester'],
        /**
         * 将 is_include 指定为 0，表示该用户组排除了“Developer”角色
         */
        array_merge($roles['Developer'], array('#JOIN#' => array('is_include' => 0))),
    )
);
$tableUserGroups->create($group);

$groups = $tableUserGroups->findAll();
$groups = array_to_hashmap($groups, 'name');

/**
 * 创建用户，并分配到各个组
 */
$tableUsers =& FLEA::getSingleton('FLEA_Acl_Table_Users');
/* @var $tableUsers FLEA_Acl_Table_Users */
$users = array(
    array(
        'username' => 'liaoyulei',
        'password' => '123456',
        'email' => 'liaoyulei@qeeyuan.com',
        'user_group_id' => $groups['QeePHP Team']['user_group_id'],
    ),
    array(
        'username' => 'liwei',
        'password' => '123456',
        'email' => 'liwei@qeeyuan.com',
        'user_group_id' => $groups['QeePHP Team']['user_group_id'],
    ),
    array(
        'username' => 'liye',
        'password' => '123456',
        'email' => 'liye@qeeyuan.com',
        'user_group_id' => $groups['QeePHP Team']['user_group_id'],
    ),
    array(
        'username' => 'dali',
        'password' => '123456',
        'email' => 'dali@qeeyuan.com',
        'user_group_id' => $groups['QeePHP Team']['user_group_id'],
    ),
);
$tableUsers->createRowset($users);

/**
 * 为用户指派单独的角色
 */
$user = $tableUsers->find(array('username' => 'liaoyulei'));
$user['roles'][] = $roles['ProjectManager'];
$tableUsers->update($user);

$user = $tableUsers->find(array('username' => 'liye'));
$user['roles'][] = $roles['Tester'];
$tableUsers->update($user);

$user = $tableUsers->find(array('username' => 'dali'));
$user['roles'][] = $roles['Tester'];
$tableUsers->update($user);



$users = array(
    array(
        'username' => '米粒子',
        'password' => '123456',
        'email' => 'milizi@phpchina.com',
        'user_group_id' => $groups['PHPChina Team']['user_group_id'],
    ),
    array(
        'username' => '默默',
        'password' => '123456',
        'email' => 'momo@phpchina.com',
        'user_group_id' => $groups['PHPChina Team']['user_group_id'],
    ),
    array(
        'username' => '冰刺猬',
        'password' => '123456',
        'email' => 'bingciwei@phpchina.com',
        'user_group_id' => $groups['PHPChina Team']['user_group_id'],
    ),
);
$tableUsers->createRowset($users);

$user = $tableUsers->find(array('username' => '米粒子'));
$user['roles'][] = $roles['ProjectManager'];
$tableUsers->update($user);

$users = array(
    array(
        'username' => '肥同小可',
        'password' => '123456',
        'email' => 'feitongxiaoke@phpchina.com',
        'user_group_id' => $groups['测试组']['user_group_id'],
    ),
    array(
        'username' => '雷茂峰',
        'password' => '123456',
        'email' => 'leimaofeng@phpchina.com',
        'user_group_id' => $groups['测试组']['user_group_id'],
    ),
);
$tableUsers->createRowset($users);

$user = $tableUsers->find(array('username' => '肥同小可'));
$user['roles'][] = $roles['Developer'];
$tableUsers->update($user);


$dbo->completeTrans();
