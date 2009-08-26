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
 * 定义 FLEA_Acl_Table_UserGroups 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: UserGroups.php 1060 2008-05-04 05:02:59Z qeeyuan $
 */

// {{{ includes
FLEA::loadClass('FLEA_Db_TableDataGateway');
// }}}

/**
 * FLEA_Acl_Table_UserGroups 类提供了用户组数据的存储服务
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Acl_Table_UserGroups extends FLEA_Db_TableDataGateway
{
    /**
     * 主键字段名
     *
     * @var string
     */
    var $primaryKey = 'user_group_id';

    /**
     * 数据表名称
     *
     * @var string
     */
    var $tableName = 'user_groups';

    /**
     * 用户组关联多个角色和权限
     *
     * @var array
     */
    var $manyToMany = array(
        array(
            'tableClass' => 'FLEA_Acl_Table_Roles',
            'foreignKey' => 'user_group_id',
            'assocForeignKey' => 'role_id',
            'joinTableClass' => 'FLEA_Acl_Table_UserGroupsHasRoles',
            'mappingName' => 'roles',
        ),
        array(
            'tableClass' => 'FLEA_Acl_Table_Permissions',
            'foreignKey' => 'user_group_id',
            'assocForeignKey' => 'permission_id',
            'joinTableClass' => 'FLEA_Acl_Table_UserGroupsHasPermissions',
            'mappingName' => 'permissions',
        ),
    );

    /**
     * 根用户组名
     *
     * @var string
     */
    var $_rootGroupName = '_#_ROOT_GROUP_#_';

    /**
     * 添加一个用户组，返回该用户组的 ID
     *
     * @param array $group
     * @param int $parentId
     *
     * @return int
     */
    function create($group, $parentId = 0) {
        $parentId = (int)$parentId;
        if ($parentId) {
            $parent = parent::find($parentId);
            if (!$parent) {
                // 指定的父用户组不存在
                FLEA::loadClass('FLEA_Acl_Exception_UserGroupNotFound');
                __THROW(new FLEA_Acl_Exception_UserGroupNotFound($parentId));
                return false;
            }
        } else {
            // 如果未指定 $parentId 为 0 或 null，则创建一个顶级用户组
            $parent = parent::find(array('name' => $this->_rootGroupName));
            if (!$parent) {
                // 如果根用户组不存在，则自动创建
                $parent = array(
                    'name' => $this->_rootGroupName,
                    'description' => '',
                    'left_value' => 1,
                    'right_value' => 2,
                    'parent_id' => -1,
                );
                if (!parent::create($parent)) {
                    return false;
                }
            }
            // 确保所有 _#_ROOT_GROUP_#_ 的直接子用户组的 parent_id 都为 0
            $parent[$this->primaryKey] = 0;
        }

        $this->dbo->startTrans();

        // 根据父用户组的左值和右值更新数据
        $sql = "UPDATE {$this->fullTableName} SET left_value = left_value + 2 " .
               "WHERE left_value >= {$parent['right_value']}";
        $this->dbo->execute($sql);
        $sql = "UPDATE {$this->fullTableName} SET right_value = right_value + 2 " .
               "WHERE right_value >= {$parent['right_value']}";
        $this->dbo->execute($sql);

        // 插入新用户组记录
        $group['left_value'] = $parent['right_value'];
        $group['right_value'] = $parent['right_value'] + 1;
        $group['parent_id'] = $parent[$this->primaryKey];
        $ret = parent::create($group);

        if ($ret) {
            $this->dbo->completeTrans();
        } else {
            $this->dbo->completeTrans(false);
        }

        return $ret;
    }

    /**
     * 更新用户组信息
     *
     * @param array $group
     *
     * @return boolean
     */
    function update($group) {
        unset($group['left_value']);
        unset($group['right_value']);
        unset($group['parent_id']);
        return parent::update($group);
    }

    /**
     * 删除一个用户组及其子用户组树
     *
     * @param int $groupId
     *
     * @return boolean
     */
    function removeByPkv($groupId) {
        $group = parent::find((int)$groupId);
        if (!$group) {
            FLEA::loadClass('FLEA_Acl_Exception_UserGroupNotFound');
            __THROW(new FLEA_Acl_Exception_UserGroupNotFound($groupId));
            return false;
        }

        $this->dbo->startTrans();

        $group['left_value'] = (int)$group['left_value'];
        $group['right_value'] = (int)$group['right_value'];
        $span = $group['right_value'] - $group['left_value'] + 1;
        $conditions = "WHERE left_value >= {$group['left_value']} AND right_value <= {$group['right_value']}";

        $rowset = $this->findAll($conditions, null, null, $this->primaryKey, false);
        foreach ($rowset as $row) {
            if (!parent::removeByPkv($row[$this->primaryKey])) {
                $this->dbo->completeTrans(false);
                return false;
            }
        }

        if (!parent::removeByPkv($groupId)) {
            $this->dbo->completeTrans(false);
            return false;
        }

        $sql = "UPDATE {$this->fullTableName} " .
               "SET left_value = left_value - {$span} " .
               "WHERE left_value > {$group['right_value']}";
        if (!$this->dbo->execute($sql)) {
            $this->dbo->completeTrans(false);
            return false;
        }

        $sql = "UPDATE {$this->fullTableName} " .
               "SET right_value = right_value - {$span} " .
               "WHERE right_value > {$group['right_value']}";
        if (!$this->dbo->execute($sql)) {
            $this->dbo->completeTrans(false);
            return false;
        }

        $this->dbo->completeTrans();
        return true;
    }

    /**
     * 返回根用户组到指定用户组路径上的所有用户组
     *
     * 返回的结果不包括“_#_ROOT_GROUP_#_”根用户组各个用户组同级别的其他用户组。
     * 结果集是一个二维数组，可以用 array_to_tree() 函数转换为层次结构（树型）。
     *
     * @param array $group
     *
     * @return array
     */
    function getPath($group) {
        $group['left_value'] = (int)$group['left_value'];
        $group['right_value'] = (int)$group['right_value'];

        $conditions = "left_value <= {$group['left_value']} AND right_value >= {$group['right_value']}";
        $sort = 'left_value ASC';
        $rowset = $this->findAll($conditions, $sort);
        if (is_array($rowset)) {
            array_shift($rowset);
        }
        return $rowset;
    }

    /**
     * 返回指定用户组的直接子用户组
     *
     * @param array $group
     *
     * @return array
     */
    function getSubGroups($group) {
        $conditions = "parent_id = {$group[$this->primaryKey]}";
        $sort = 'left_value ASC';
        return $this->findAll($conditions, $sort);
    }

    /**
     * 返回指定用户组为根的整个子用户组树
     *
     * @param array $group
     *
     * @return array
     */
    function getSubTree($group) {
        $group['left_value'] = (int)$group['left_value'];
        $group['right_value'] = (int)$group['right_value'];

        $conditions = "left_value BETWEEN {$group['left_value']} AND {$group['right_value']}";
        $sort = 'left_value ASC';
        return $this->findAll($conditions, $sort);
    }

    /**
     * 获取指定用户组同级别的所有用户组
     *
     * @param array $group
     *
     * @return array
     */
    function getCurrentLevelGroups($group) {
        $group['parent_id'] = (int)$group['parent_id'];
        $conditions = "parent_id = {$group['parent_id']}";
        $sort = 'left_value ASC';
        return $this->findAll($conditions, $sort);
    }

    /**
     * 取得所有用户组
     *
     * @return array
     */
    function getAllGroups() {
        return parent::findAll('left_value > 1', 'left_value ASC');
    }

    /**
     * 获取所有顶级用户组（既 _#_ROOT_GROUP_#_ 的直接子用户组）
     *
     * @return array
     */
    function getAllTopGroups() {
        $conditions = "parent_id = 0";
        $sort = 'left_value ASC';
        return $this->findAll($conditions, $sort);
    }

    /**
     * 计算所有子用户组的总数
     *
     * @param array $group
     *
     * @return int
     */
    function calcAllChildCount($group) {
        return intval(($group['right_value'] - $group['left_value'] - 1) / 2);
    }

}
