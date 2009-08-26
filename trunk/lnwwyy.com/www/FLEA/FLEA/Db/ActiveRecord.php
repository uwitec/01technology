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
 * 定义 FLEA_Db_ActiveRecord 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技(www.qeeyuan.com)
 * @package Core
 * @version $Id: ActiveRecord.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Db_ActiveRecord 类实现了 ActiveRecord 模式
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技(www.qeeyuan.com)
 * @package Core
 * @version $Id: ActiveRecord.php 972 2007-10-09 20:56:54Z qeeyuan $
 */
class FLEA_Db_ActiveRecord
{
    /**
     * 定义该对象要聚合的其他对象
     *
     * @var array
     */
    var $_aggregation = array();

    /**
     * 用于完成数据库操作的 TableDataGateway 继承类
     *
     * @var FLEA_Db_TableDataGateway
     */
    var $_table;

    /**
     * 该对象的主键属性名
     *
     * @var string
     */
    var $_idname;

    /**
     * 指示该对象是否已经初始化
     *
     * @var boolean
     */
    var $init = false;

    /**
     * 字段和对象属性之间的映射关系
     *
     * @var array
     */
    var $_mapping = false;

    /**
     * 继承类必须覆盖此静态函数
     *
     * @static
     *
     * @return array
     */
    function define()
    {
    }

    /**
     * 构造函数
     *
     * 根据 $conditions 参数查询符合条件的记录作为对象属性。
     *
     * @param mixed $conditions
     *
     * @return FLEA_Db_ActiveRecord
     */
    function FLEA_Db_ActiveRecord($conditions = null)
    {
        $this->init();
        $this->load($conditions);
    }

    /**
     * 初始化
     *
     * @param array $options
     */
    function init()
    {
        if ($this->init) { return; }
        $this->init = true;

        $myclass = get_class($this);
        $options = call_user_func(array($myclass, 'define'));
        $tableClass = $options['tableClass'];

        $objid = "{$myclass}_tdg";
        if (FLEA::isRegistered($objid)) {
            $this->_table =& FLEA::registry($objid);
        } else {
            FLEA::loadClass($tableClass);
            $this->_table =& new $tableClass(array('skipCreateLinks' => true));
            FLEA::register($this->_table, $objid);
        }

        if (!empty($options['propertiesMapping'])) {
            $this->_mapping = array(
                'p2f' => $options['propertiesMapping'],
                'f2p' => array_flip($options['propertiesMapping']),
            );
            $this->_idname = $this->_mapping['f2p'][$this->_table->primaryKey];
        } else {
            $this->_mapping = array('p2f' => array(), 'f2p' => array());
            foreach ($this->_table->meta as $field) {
                $this->_mapping['p2f'][$field['name']] = $field['name'];
                $this->_mapping['f2p'][$field['name']] = $field['name'];
            }
            $this->_idname = $this->_table->primaryKey;
        }

        if (!isset($options['aggregation']) || !is_array($options['aggregation'])) {
            $options['aggregation'] = array();
        }
        foreach ($options['aggregation'] as $offset => $define) {
            if (!isset($define['mappingName'])) {
                $define['mappingName'] = substr(strtolower($define['tableClass']), 0, 1) . substr($define['tableClass'], 1);
            }
            if ($define['mappingType'] == HAS_MANY || $define['mappingType'] == MANY_TO_MANY) {
                $this->{$define['mappingName']} = array();
            } else {
                $this->{$define['mappingName']} = null;
            }

            /**
             * 获得聚合对象的定义信息
             */
            FLEA::loadClass($define['class']);
            $options = call_user_func(array($define['class'], 'define'));

            $link = array(
                'tableClass' => $options['tableClass'],
                'mappingName' => $define['mappingName'],
                'foreignKey' => isset($define['foreignKey']) ? $define['foreignKey'] : null,
            );

            if ($define['mappingType'] == MANY_TO_MANY) {
                $link['joinTable'] = isset($define['joinTable']) ? $define['joinTable'] : null;
                $link['assocForeignKey'] = isset($define['assocForeignKey']) ? $define['assocForeignKey'] : null;
            }

            $this->_table->createLink($link, $define['mappingType']);
            $define['link'] =& $this->_table->getLink($link['mappingName']);
            $this->_aggregation[$offset] = $define;
        }
    }

    /**
     * 从数据库载入符合条件的一个对象
     *
     * @param mixed $conditions
     */
    function load($conditions)
    {
        $row = $this->_table->find($conditions);
        if (is_array($row)) { $this->attach($row); }
    }

    /**
     * 保存对象到数据库
     */
    function save()
    {
        $row =& $this->toArray();
        $this->_table->save($row);
    }

    /**
     * 从数据库删除对象
     */
    function delete()
    {
        $this->_table->removeByPkv($this->getId());
    }

    /**
     * 设置对象主键值
     *
     * @param mixed $id
     */
    function setId($id)
    {
        $this->{$this->_idname} = $id;
    }

    /**
     * 返回对象主键值
     *
     * @return mixed
     */
    function getId()
    {
        return $this->{$this->_idname};
    }

    /**
     * 将对象属性转换为数组
     *
     * @return array
     */
    function toArray()
    {
        $arr = array();
        foreach ($this->_mapping['p2f'] as $prop => $field) {
            $arr[$field] = $this->{$prop};
        }
        return $arr;
    }

    /**
     * 将记录的值绑定到对象
     *
     * @param array $row
     */
    function attach(& $row)
    {
        foreach ($this->_mapping['f2p'] as $field => $prop) {
            if (isset($row[$field])) {
                $this->{$prop} = $row[$field];
            }
        }

        foreach ($this->_aggregation as $define) {
            $mn = $define['link']->mappingName;
            if (!isset($row[$mn])) { continue; }
            if ($define['link']->oneToOne) {
                $this->{$mn} =& new $define['class']($row[$mn]);
            } else {
                $this->{$mn}[] =& new $define['class']($row[$mn]);
            }
        }
    }

}
