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
 * 定义 FLEA_Db_TableDataGateway 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: TableDataGateway.php 1401 2008-10-08 03:00:38Z yangkun $
 */

// {{{ includes
FLEA::loadClass('FLEA_Db_TableLink');
// }}}

// {{{ constants
/**
 * HAS_ONE 关联表示一个记录拥有另一个关联的记录
 */
define('HAS_ONE',       1);

/**
 * BELONGS_TO 关联表示一个记录属于另一个记录
 */
define('BELONGS_TO',    2);

/**
 * HAS_MANY 关联表示一个记录拥有多个关联的记录
 */
define('HAS_MANY',      3);

/**
 * MANY_TO_MANY 关联表示两个数据表的数据互相引用
 */
define('MANY_TO_MANY',  4);
// }}}

/**
 * FLEA_Db_TableDataGateway 类（表数据入口）封装了数据表的 CRUD 操作
 *
 * 开发者应该从 FLEA_Db_TableDataGateway 派生自己的类，
 * 并通过添加方法来封装针对该数据表的更复杂的数据库操作。
 *
 * 对于每一个表数据入口对象，都必须在类定义中通过 $tableName 和 $primaryKey
 * 来分别指定数据表的名字和主键字段名。
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.2
 */
class FLEA_Db_TableDataGateway
{
    /**
     * 数据库 schema
     *
     * @var string
     */
    var $schema = '';

    /**
     * 数据表名（没有添加前缀）
     *
     * @var string
     */
    var $tableName = null;

    /**
     * 包含前缀的完整数据表名称
     *
     * @var string
     */
    var $fullTableName = null;

    /**
     * 主键字段名，或者是包含多个主键字段名的数组
     *
     * @var sring|array
     */
    var $primaryKey = null;

    /**
     * 定义一对一关联
     *
     * @var array
     */
    var $hasOne = null;

    /**
     * 定义从属关联
     *
     * @var array
     */
    var $belongsTo = null;

    /**
     * 定义一对多关联
     *
     * @var array
     */
    var $hasMany = null;

    /**
     * 定义多对多关联
     *
     * @var array
     */
    var $manyToMany = null;

    /**
     * 当前数据表的元数据
     *
     * 元数据是一个二维数组，每一个元素的键名就是全大写的字段名，
     * 而键值则是该字段的数据表定义。
     *
     * @var array
     */
    var $meta = null;

    /**
     * 当前数据表的所有字段名
     *
     * @var array
     */
    var $fields = null;

    /**
     * 指示是否对数据进行自动验证
     *
     * 当 autoValidating 为 true 时，create() 和 update() 方法将对数据进行验证。
     *
     * @var boolean
     */
    var $autoValidating = false;

    /**
     * 用于数据验证的对象
     *
     * @var FLEA_Helper_Verifier
     */
    var $verifier = null;

    /**
     * 附加的验证规则
     *
     * @var array
     */
    var $validateRules = null;

    /**
     * 创建记录时，要自动填入当前时间的字段
     *
     * 只要数据表具有下列字段之一，则调用 create() 方法创建记录时，
     * 将以服务器时间自动填充该字段。
     *
     * @var array
     */
    var $createdTimeFields = array('CREATED', 'CREATED_ON', 'CREATED_AT');

    /**
     * 创建和更新记录时，要自动填入当前时间的字段
     *
     * 只要数据表具有下列字段之一，则调用 create() 方法创建记录或 update() 更新记录时，
     * 将以服务器时间自动填充该字段。
     *
     * @var array
     */
    var $updatedTimeFields = array('UPDATED', 'UPDATED_ON', 'UPDATED_AT');

    /**
     * 指示进行 CRUD 操作时是否处理关联
     *
     * 开发者应该使用 enableLinks() 和 disableLinks() 方法来启用或禁用关联处理。
     *
     * @var boolean
     */
    var $autoLink = true;

    /**
     * 数据库访问对象
     *
     * 开发者不应该直接访问该成员变量，而是通过 setDBO() 和 getDBO() 方法
     * 来访问表数据入口使用数据访问对象。
     *
     * @var FLEA_Db_Driver_Abstract
     */
    var $dbo = null;

    /**
     * 存储关联信息
     *
     * $links 是一个数组，数组中保存 TableLink 对象。
     * 开发者应该使用 getLink() 和 createLink() 等方法来访问这些关联对象。
     *
     * @var array
     */
    var $links = array();

    /**
     * 包含前缀的数据表完全限定名
     *
     * @var string
     * @access private
     */
    var $qtableName;

    /**
     * 主键字段完全限定名
     *
     * @var string
     * @access private
     */
    var $qpk;

    /**
     * 用于关联查询时的主键字段别名
     */
    var $pka;

    /**
     * 用于关联查询时的主键字段完全限定名
     *
     * @var string
     * @access private
     */
    var $qpka;

    /**
     * 保存最后一次数据验证的结果
     *
     * 调用 getLastValidation() 方法可以获得最后一次数据验证的结果。
     *
     * @var array
     */
    var $lastValidationResult;

    /**
     * 构造 FLEA_Db_TableDataGateway 实例
     *
     * $params 参数允许有下列选项：
     *   - schema: 指定数据表的 schema
     *   - tableName: 指定数据表的名称；
     *   - primaryKey: 指定主键字段名
     *   - autoValidating: 指示是否使用自动验证；
     *   - verifierProvider: 指定要使用的数据验证服务对象。
     *     如果未指定。则使用应用程序设置 helper.verifier 指定的验证服务提供对象；
     *   - skipConnect: 指示初始化表数据入口对象时是否不连接到数据库；
     *   - dbDSN: 指定连接数据库要使用的 DSN，如果未指定则使用默认的 DSN 设置；
     *   - dbo: 指定要使用的数据库访问对象;
     *   - skipCreateLinks: 指示初始化表数据入口时，是否不建立关联关系
     *
     * @param array $params
     *
     * @return FLEA_Db_TableDataGateway
     */
    function FLEA_Db_TableDataGateway($params = null)
    {
        if (!empty($params['schema'])) {
            $this->schema = $params['schema'];
        }
        if (!empty($params['tableName'])) {
            $this->tableName = $params['tableName'];
        }

        if (!empty($params['primaryKey'])) {
            $this->primaryKey = $params['primaryKey'];
        }

        // 初始化验证服务对象
        if (isset($params['autoValidating'])) {
            $this->autoValidating = $params['autoValidating'];
        }
        if ($this->autoValidating) {
            if (!empty($params['verifierProvider'])) {
                $provider = $params['verifierProvider'];
            } else {
                $provider = FLEA::getAppInf('helper.verifier');
            }
            if (!empty($provider)) {
                $this->verifier =& FLEA::getSingleton($provider);
            }
        }

        // 当 skipInit 为 true 时，不初始化表数据入口对象
        if (isset($params['skipConnect']) && $params['skipConnect'] != false) {
            return;
        }

        // 初始化数据访问对象
        if (!isset($params['dbo'])) {
            if (isset($params['dbDSN'])) {
                $dbo =& FLEA::getDBO($params['dbDSN']);
            } else {
                $dbo =& FLEA::getDBO();
            }
        } else {
            $dbo =& $params['dbo'];
        }
        $this->setDBO($dbo);

        // 当 skipCreateLinks 不为 true 时，建立关联
        if (!isset($params['skipCreateLinks']) || $params['skipCreateLinks'] == false) {
            $this->relink();
        }
    }

    /**
     * 设置数据库访问对象
     *
     * @param FLEA_Db_Driver_Abstract $dbo
     *
     * @return boolean
     */
    function setDBO(& $dbo)
    {
        $this->dbo =& $dbo;

        if (empty($this->schema) && !empty($dbo->dsn['schema'])) {
            $this->schema = $dbo->dsn['schema'];
        }
        if (empty($this->fullTableName)) {
            $this->fullTableName = $dbo->dsn['prefix'] . $this->tableName;
        }
        $this->qtableName = $dbo->qtable($this->fullTableName, $this->schema);

        if (!$this->_prepareMeta()) {
            return false;
        }
        $this->fields = array_keys($this->meta);

        if (is_array($this->validateRules)) {
            foreach ($this->validateRules as $fieldName => $rules) {
                $fieldName = strtoupper($fieldName);
                if (!isset($this->meta[$fieldName])) { continue; }
                foreach ((array)$rules as $ruleName => $rule) {
                    $this->meta[$fieldName][$ruleName] = $rule;
                }
            }
        }

        // 如果没有指定主键，则尝试自动获取
        if (empty($this->primaryKey)) {
            foreach ($this->meta as $field) {
                if ($field['primaryKey']) {
                    $this->primaryKey = $field['name'];
                    break;
                }
            }
        }

        if (is_array($this->primaryKey)) {
            $this->qpk = array();
            $this->pka = array();
            $this->qpka = array();
            foreach ($this->primaryKey as $pk) {
                $qpk = $dbo->qfield($pk, $this->fullTableName, $this->schema);
                $this->qpk[$pk] = $qpk;
                $pka = 'flea_pkref_' . $pk;
                $this->pka[$pk] = $pka;
                $this->qpka[$pk] = $qpk . ' AS ' . $pka;
            }
        } else {
            $this->qpk = $dbo->qfield($this->primaryKey, $this->fullTableName, $this->schema);
            $this->pka = 'flea_pkref_' . $this->primaryKey;
            $this->qpka = $this->qpk . ' AS ' . $this->pka;
        }

        return true;
    }

    /**
     * 返回该表数据入口对象使用的数据访问对象
     *
     * @return FLEA_Db_Driver_Abstract
     */
    function & getDBO()
    {
        return $this->dbo;
    }

    /**
     * 返回符合条件的第一条记录及所有关联的数据，查询没有结果返回 false
     *
     * @param mixed $conditions
     * @param string $sort
     * @param mixed $fields
     * @param mixed $queryLinks
     *
     * @return array
     */
    function & find($conditions, $sort = null, $fields = '*', $queryLinks = true)
    {
        $rowset =& $this->findAll($conditions, $sort, 1, $fields, $queryLinks);
        if (is_array($rowset)) {
            $row = reset($rowset);
        } else {
            $row = false;
        }
        unset($rowset);
        return $row;
    }

    /**
     * 查询所有符合条件的记录及相关数据，返回一个包含多行记录的二维数组，失败时返回 false
     *
     * @param mixed $conditions
     * @param string $sort
     * @param mixed $limit
     * @param mixed $fields
     * @param mixed $queryLinks
     *
     * @return array
     */
    function & findAll($conditions = null, $sort = null, $limit = null, $fields = '*', $queryLinks = true)
    {
        list($whereby, $distinct) = $this->getWhere($conditions);
        // 处理排序
        $sortby = $sort != '' ? " ORDER BY {$sort}" : '';
        // 处理 $limit
        if (is_array($limit)) {
            list($length, $offset) = $limit;
        } else {
            $length = $limit;
            $offset = null;
        }

        // 构造从主表查询数据的 SQL 语句
        $enableLinks = count($this->links) > 0 && $this->autoLink && $queryLinks;
        $fields = $this->dbo->qfields($fields, $this->fullTableName, $this->schema);
        if ($enableLinks) {
            // 当有关联需要处理时，必须获得主表的主键字段值
            $sql = "SELECT {$distinct} {$this->qpka}, {$fields} FROM {$this->qtableName} {$whereby} {$sortby}";
        } else {
            $sql = "SELECT {$distinct} {$fields} FROM {$this->qtableName} {$whereby} {$sortby}";
        }

        // 根据 $length 和 $offset 参数决定是否使用限定结果集的查询
        if (null !== $length || null !== $offset) {
            $result = $this->dbo->selectLimit($sql, $length, $offset);
        } else {
            $result = $this->dbo->execute($sql);
        }

        if ($enableLinks) {
            /**
             * 查询时同时将主键值单独提取出来，
             * 并且准备一个以主键值为键名的二维数组用于关联数据的装配
             */
            $pkvs = array();
            $assocRowset = null;
            $rowset = $this->dbo->getAllWithFieldRefs($result, $this->pka, $pkvs, $assocRowset);
            $in = 'IN (' . implode(',', array_map(array(& $this->dbo, 'qstr'), $pkvs)) . ')';
        } else {
            $rowset = $this->dbo->getAll($result);
        }
        unset($result);

        // 如果没有关联需要处理或者没有查询结果，则直接返回查询结果
        if (!$enableLinks || empty($rowset) || !$this->autoLink) {
            return $rowset;
        }

        /**
         * 遍历每一个关联对象，并从关联对象获取查询语句
         *
         * 查询获得数据后，将关联表的数据和主表数据装配在一起
         */
        $callback = create_function('& $r, $o, $m', '$r[$m] = null;');
        foreach ($this->links as $link) {
            /* @var $link FLEA_Db_TableLink */
            $mn = $link->mappingName;
            if (!$link->enabled || !$link->linkRead) { continue; }
            if (!$link->countOnly) {
                array_walk($assocRowset, $callback, $mn);
                $sql = $link->getFindSQL($in);
                $this->dbo->assemble($sql, $assocRowset, $mn, $link->oneToOne, $this->pka, $link->limit);
            } else {
                $link->calcCount($assocRowset, $mn, $in);
            }
        }

        return $rowset;
    }

    /**
     * 对单一记录进行递归查询，并将查询结果组装到记录中
     *
     * @param string $mappingName
     * @param array $row
     * @param array $enabledLinks
     */
    function assembleRecursionRow($mappingName, & $row, $enabledLinks = null)
    {
        $assoclink =& $this->getLink($mappingName);
        if ($assoclink == false) { return false; }

        $assoclink->init();
        $tdg =& $assoclink->assocTDG;
        $arow =& $row[$mappingName];

        if (!is_array($enabledLinks)) {
            if ($enabledLinks == null) {
                $enabledLinks = array_keys($tdg->links);
            } else {
                $enabledLinks = explode(',', $enabledLinks);
                array_walk($enabledLinks, 'trim');
                $enabledLinks = array_filter($enabledLinks, 'strlen');
            }
        }
        $enabledLinks = array_flip($enabledLinks);
        $enabledLinks = array_change_key_case($enabledLinks, CASE_LOWER);
        $this->enableLinks(array_keys($enabledLinks));

        foreach ($tdg->links as $link) {
            /* @var $link FLEA_Db_TableLink */
            if (!$link->enabled || !$link->linkRead || !isset($enabledLinks[$link->mappingName])) { continue; }

            $in = array();
            switch ($assoclink->type) {
            case HAS_ONE:
            case BELONGS_TO:
                $pkv = $arow[$link->mainTDG->primaryKey];
                $in[] = $pkv;
                $assocRowset = array($pkv => & $arow);
                $arow[$link->mappingName] = null;
                break;
            case HAS_MANY:
            case MANY_TO_MANY:
                $assocRowset = array();
                foreach (array_keys($arow) as $offset) {
                    $pkv = $arow[$offset][$link->mainTDG->primaryKey];
                    $in[] = $pkv;
                    $assocRowset[$pkv] = & $arow[$offset];
                    $arow[$offset][$link->mappingName] = null;
                }
            }
            if (empty($in)) { continue; }

            $in = 'IN (' . implode(',', array_map(array(& $this->dbo, 'qstr'), $in)) . ')';

            $sql = $link->getFindSQL($in);
            $this->dbo->assemble($sql, $assocRowset, $link->mappingName, $link->oneToOne, $link->mainTDG->pka, $link->limit);
        }

        return true;
    }


    /**
     * 对多行记录集递归查询，并将查询结果组装到记录中
     *
     * @param string $mappingName
     * @param array $rowset
     * @param array $enabledLinks
     */
    function assembleRecursionRowset($mappingName, & $rowset, $enabledLinks = null)
    {
        $assoclink =& $this->getLink($mappingName);
        if ($assoclink == false) { return false; }

        $assoclink->init();
        $tdg =& $assoclink->assocTDG;
        $arowset = array();
        foreach (array_keys($rowset) as $offset) {
            $arowset[] =& $rowset[$offset][$mappingName];
        }
        $keys = array_keys($arowset);

        if (!is_array($enabledLinks)) {
            if ($enabledLinks == null) {
                $enabledLinks = array_keys($tdg->links);
            } else {
                $enabledLinks = explode(',', $enabledLinks);
                array_walk($enabledLinks, 'trim');
                $enabledLinks = array_filter($enabledLinks, 'strlen');
            }
        }
        $enabledLinks = array_flip($enabledLinks);
        $enabledLinks = array_change_key_case($enabledLinks, CASE_LOWER);
        $this->enableLinks(array_keys($enabledLinks));

        foreach ($tdg->links as $link) {
            /* @var $link FLEA_Db_TableLink */
            if (!$link->enabled || !$link->linkRead || !isset($enabledLinks[$link->mappingName])) { continue; }

            $in = array();
            $assocRowset = array();
            switch ($assoclink->type) {
            case HAS_ONE:
            case BELONGS_TO:
                foreach ($keys as $key) {
                    $pkv = $arowset[$key][$link->mainTDG->primaryKey];
                    $in[] = $pkv;
                    $assocRowset[$pkv] =& $arowset[$key];
                    $arowset[$key][$link->mappingName] = null;
                }
                break;
            case HAS_MANY:
            case MANY_TO_MANY:
                foreach ($keys as $key) {
                    foreach (array_keys($arowset[$key]) as $offset) {
                        $pkv = $arowset[$key][$offset][$link->mainTDG->primaryKey];
                        $in[] = $pkv;
                        $assocRowset[$pkv] = & $arowset[$key][$offset];
                        $arow[$key][$offset][$link->mappingName] = null;
                    }
                }
            }
            $in = 'IN (' . implode(',', array_map(array(& $this->dbo, 'qstr'), $in)) . ')';

            $sql = $link->getFindSQL($in);
            $this->dbo->assemble($sql, $assocRowset, $link->mappingName, $link->oneToOne, $link->mainTDG->pka, $link->limit);
        }

        return true;
    }

    /**
     * 返回具有指定字段值的第一条记录
     *
     * @param string $field
     * @param mixed $value
     * @param string $sort
     * @param mixed $fields
     *
     * @return array
     */
    function & findByField($field, $value, $sort = null, $fields = '*')
    {
        return $this->find(array($field => $value), $sort, $fields);
    }

    /**
     * 返回具有指定字段值的所有记录
     *
     * @param string $field
     * @param mixed $value
     * @param string $sort
     * @param array $limit
     * @param mixed $fields
     *
     * @return array
     */
    function & findAllByField($field, $value, $sort = null, $limit = null, $fields = '*')
    {
        return $this->findAll(array($field => $value), $sort, $limit, $fields);
    }

    /**
     * 以包含主键值的数组为条件查询记录集
     *
     * @param array $pkvs
     * @param mixed $conditions
     * @param string $sort
     * @param mixed $limit
     * @param mixed $fields
     * @param mixed $queryLinks
     *
     * @return array
     */
    function & findAllByPkvs($pkvs, $conditions = null, $sort = null, $limit = null, $fields = '*', $queryLinks = true)
    {
        $in = array('in()' => $pkvs);
        if (empty($conditions)) {
            $conditions = $in;
        } else {
            if (!is_array($conditions)) {
                $conditions = array($in, $conditions);
            } else {
                array_push($conditions, $in);
            }
        }

        return $this->findAll($conditions, $sort, $limit, $fields, $queryLinks);
    }

    /**
     * 直接使用 sql 语句获取记录（该方法不会处理关联数据表）
     *
     * @param string $sql
     * @param mixed $limit
     *
     * @return array
     */
    function & findBySql($sql, $limit = null)
    {
        // 处理 $limit
        if (is_array($limit)) {
            list($length, $offset) = $limit;
        } else {
            $length = $limit;
            $offset = null;
        }
        if (is_null($length) && is_null($offset)) {
            return $this->dbo->getAll($sql);
        }

        $result = $this->dbo->selectLimit($sql, $length, $offset);
        if ($result) {
            $rowset = $this->dbo->getAll($result);
        } else {
            $rowset = false;
        }
        return $rowset;
    }

    /**
     * 统计符合条件的记录的总数
     *
     * @param mixed $conditions
     * @param string|array $fields
     *
     * @return int
     */
    function findCount($conditions = null, $fields = null)
    {
        list($whereby, $distinct) = $this->getWhere($conditions);
        if (is_null($fields)) {
            $fields = $this->qpk;
        } else {
            $fields = $this->dbo->qfields($fields, $this->fullTableName);
        }
        $sql = "SELECT {$distinct}COUNT({$fields}) FROM {$this->qtableName}{$whereby}";
        return (int)$this->dbo->getOne($sql);
    }

    /**
     * 保存数据到数据库
     *
     * 如果数据包含主键值，则 save() 会调用 update() 来更新记录，否则调用 create() 来创建记录。
     *
     * @param array $row
     * @param boolean $saveLinks
     * @param boolean $updateCounter
     *
     * @return boolean
     */
    function save(& $row, $saveLinks = true, $updateCounter = true)
    {
        if (empty($row[$this->primaryKey])) {
            return $this->create($row, $saveLinks, $updateCounter);
        } else {
            return $this->update($row, $saveLinks, $updateCounter);
        }
    }

    /**
     * 保存一个记录集（多行数据）
     *
     * @param array $rowset
     * @param boolean $saveLinks
     *
     * @return boolean
     */
    function saveRowset(& $rowset, $saveLinks = true)
    {
        $this->dbo->startTrans();
        foreach ($rowset as $row) {
            if (!$this->save($row, $saveLinks, false)) {
                $this->dbo->completeTrans(false);
                return false;
            }
        }
        $this->dbo->completeTrans();
        return true;
    }

    /**
     * 替换一条现有记录或插入新记录，返回记录的主键值，失败返回 false
     *
     * @param array $row
     *
     * @return mixed
     */
    function replace(& $row) {
        $this->_setCreatedTimeFields($row);
        $fields = '';
        $values = '';
        foreach ($row as $field => $value) {
            if (!isset($this->meta[strtoupper($field)])) { continue; }
            $fields .= $this->dbo->qfield($field) . ', ';
            $values .= $this->dbo->qstr($value) . ', ';
        }
        $fields = substr($fields, 0, -2);
        $values = substr($values, 0, -2);
        $sql = "REPLACE INTO {$this->fullTableName} ({$fields}) VALUES ({$values})";
        if (!$this->dbo->execute($sql)) { return false; }

        if (!empty($row[$this->primaryKey])) {
            return $row[$this->primaryKey];
        }

        $insertid = $this->dbo->insertId();
        return $insertid;
    }

    /**
     * 替换记录集（多行数据），返回记录集的主键字段值，失败返回 false
     *
     * @param array $rowset
     *
     * @return array
     */
    function replaceRowset(& $rowset)
    {
        $ids = array();
        $this->dbo->startTrans();
        foreach ($rowset as $row) {
            $id = $this->replace($row, false);
            if (!$id) {
                $this->dbo->completeTrans(false);
                return false;
            }
            $ids[] = $id;
        }
        $this->dbo->completeTrans();
        return $ids;
    }

    /**
     * 更新一条现有的记录，成功返回 true，失败返回 false
     *
     * 该操作会引发 _beforeUpdate()、_beforeUpdateDb() 和 _afterUpdateDb() 事件。
     *
     * @param array $row
     * @param boolean $saveLinks
     *
     * @return boolean
     */
    function update(& $row, $saveLinks = true)
    {
        if (!$this->_beforeUpdate($row)) {
            return false;
        }

        // 检查是否提供了主键值
        if (!isset($row[$this->primaryKey])) {
            FLEA::loadClass('FLEA_Db_Exception_MissingPrimaryKey');
            return __THROW(new FLEA_Db_Exception_MissingPrimaryKey($this->primaryKey));
        }

        // 自动填写记录的最后更新时间字段
        $this->_setUpdatedTimeFields($row);

        // 如果提供了验证器，则进行数据验证
        if ($this->autoValidating && !is_null($this->verifier)) {
            if (!$this->checkRowData($row, true)) {
                // 验证失败抛出异常
                FLEA::loadClass('FLEA_Exception_ValidationFailed');
                return __THROW(new FLEA_Exception_ValidationFailed($this->getLastValidation(), $row));
            }
        }

        // 开始事务
        $this->dbo->startTrans();

        // 调用 _beforeUpdateDb() 事件
        if (!$this->_beforeUpdateDb($row)) {
            $this->dbo->completeTrans(false);
            return false;
        }

        // 生成 SQL 语句
        $pkv = $row[$this->primaryKey];
        unset($row[$this->primaryKey]);
        list($pairs, $values) = $this->dbo->getPlaceholderPair($row, $this->fields);
        $row[$this->primaryKey] = $pkv;

        if (!empty($pairs)) {
            $pairs = implode(',', $pairs);
            $sql = "UPDATE {$this->qtableName} SET {$pairs} WHERE {$this->qpk} = " . $this->dbo->qstr($pkv);

            // 执行更新操作
            if (!$this->dbo->execute($sql, $values)) {
                $this->dbo->completeTrans(false);
                return false;
            }
        }

        // 处理对关联数据的更新
        if ($this->autoLink && $saveLinks) {
            foreach (array_keys($this->links) as $linkKey) {
                $link =& $this->links[$linkKey];
                /* @var $link FLEA_Db_TableLink */
                // 跳过不需要处理的关联
                if (!$link->enabled || !$link->linkUpdate || !isset($row[$link->mappingName]) || !is_array($row[$link->mappingName])) {
                    continue;
                }

                if (!$link->saveAssocData($row[$link->mappingName], $pkv)) {
                    $this->dbo->completeTrans(false);
                    return false;
                }
            }
        }

        $this->_updateCounterCache($row);

        // 提交事务
        $this->dbo->completeTrans();

        $this->_afterUpdateDb($row);

        return true;
    }

    /**
     * 更新记录集（多行记录）
     *
     * @param array $rowset
     * @param boolean $saveLinks
     *
     * @return boolean
     */
    function updateRowset(& $rowset, $saveLinks = true)
    {
        $this->dbo->startTrans();
        foreach ($rowset as $row) {
            if (!$this->update($row, $saveLinks, false)) {
                $this->dbo->completeTrans(false);
                return false;
            }
        }
        $this->dbo->completeTrans();
        return true;
    }

    /**
     * 更新符合条件的记录，成功返回更新的记录总数，失败返回 false
     *
     * 该操作不会引发任何事件，也不会处理关联数据。
     *
     * @param mixed $conditions
     * @param array $$row
     *
     * @return int|boolean
     */
    function updateByConditions($conditions, & $row)
    {
        $whereby = $this->getWhere($conditions, false);
        $this->_setUpdatedTimeFields($row);

        list($pairs, $values) = $this->dbo->getPlaceholderPair($row, $this->fields);
        $pairs = implode(',', $pairs);
        $sql = "UPDATE {$this->qtableName} SET {$pairs} {$whereby}";
        return $this->dbo->execute($sql, $values);
    }

    /**
     * 更新记录的指定字段，返回更新的记录总数
     *
     * 该操作不会引发任何事件，也不会处理关联数据。
     *
     * @param mixed $conditions
     * @param string $field
     * @param mixed $value
     *
     * @return int
     */
    function updateField($conditions, $field, $value)
    {
        $row = array($field => $value);
        return $this->updateByConditions($conditions, $row);
    }

    /**
     * 增加符合条件的记录的指定字段的值，返回更新的记录总数
     *
     * 该操作不会引发任何事件，也不会处理关联数据。
     *
     * @param mixed $conditions
     * @param string $field
     * @param int $incr
     *
     * @return mixed
     */
    function incrField($conditions, $field, $incr = 1)
    {
        $field = $this->dbo->qfield($field, $this->fullTableName, $this->schema);
        $incr = (int)$incr;

        $row = array();
        $this->_setUpdatedTimeFields($row);
        list($pairs, $values) = $this->dbo->getPlaceholderPair($row, $this->fields);
        $pairs = implode(',', $pairs);
        if ($pairs) {
            $pairs = ', ' . $pairs;
        }

        $whereby = $this->getWhere($conditions, false);
        $sql = "UPDATE {$this->qtableName} SET {$field} = {$field} + {$incr}{$pairs} {$whereby}";
        return $this->dbo->execute($sql, $values);
    }

    /**
     * 减小符合条件的记录的指定字段的值，返回更新的记录总数
     *
     * 该操作不会引发任何事件，也不会处理关联数据。
     *
     * @param mixed $conditions
     * @param string $field
     * @param int $decr
     *
     * @return mixed
     */
    function decrField($conditions, $field, $decr = 1)
    {
        $field = $this->dbo->qfield($field, $this->fullTableName, $this->schema);
        $decr = (int)$decr;

        $row = array();
        $this->_setUpdatedTimeFields($row);
        list($pairs, $values) = $this->dbo->getPlaceholderPair($row, $this->fields);
        $pairs = implode(',', $pairs);
        if ($pairs) {
            $pairs = ', ' . $pairs;
        }

        $whereby = $this->getWhere($conditions, false);
        $sql = "UPDATE {$this->qtableName} SET {$field} = {$field}- {$decr}{$pairs} {$whereby}";
        return $this->dbo->execute($sql, $values);
    }

    /**
     * 插入一条新记录，返回新记录的主键值
     *
     * create() 操作会引发 _beforeCreate()、_beforeCreateDb() 和 _afterCreateDb() 事件。
     *
     * @param array $row
     * @param boolean $saveLinks
     *
     * @return mixed
     */
    function create(& $row, $saveLinks = true)
    {
        if (!$this->_beforeCreate($row)) {
            return false;
        }

        // 自动设置日期字段
        $this->_setCreatedTimeFields($row);

        // 处理主键
        $mpk = strtoupper($this->primaryKey);
        $insertId = null;
        $unsetpk = true;
        if (isset($this->meta[$mpk]['autoIncrement']) && $this->meta[$mpk]['autoIncrement'])
        {
            if (isset($row[$this->primaryKey])) {
                if (empty($row[$this->primaryKey])) {
                    // 如果主键字段是自增，而提供的记录数据虽然包含主键字段，
                    // 但却是空值，则删除这个空值
                    unset($row[$this->primaryKey]);
                } else {
                    $unsetpk = false;
                }
            }
        } else {
            // 如果主键字段不是自增字段，并且没有提供主键字段值时，则获取一个新的主键字段值
            if (!isset($row[$this->primaryKey]) || empty($row[$this->primaryKey])) {
                $insertId = $this->newInsertId();
                $row[$this->primaryKey] = $insertId;
            } else {
                // 使用开发者提交的主键字段值
                $insertId = $row[$this->primaryKey];
                $unsetpk = false;
            }
        }

        // 自动验证数据
        if ($this->autoValidating && !is_null($this->verifier)) {
            if (!$this->checkRowData($row)) {
                FLEA::loadClass('FLEA_Exception_ValidationFailed');
                __THROW(new FLEA_Exception_ValidationFailed($this->getLastValidation(), $row));
                return false;
            }
        }

        // 调用 _beforeCreateDb() 事件
        $this->dbo->startTrans();

        if (!$this->_beforeCreateDb($row)) {
            if ($unsetpk) { unset($row[$this->primaryKey]); }
            $this->dbo->completeTrans(false);
            return false;
        }

        // 生成 SQL 语句
        list($holders, $values) = $this->dbo->getPlaceholder($row, $this->fields);
        $holders = implode(',', $holders);
        $fields = $this->dbo->qfields(array_keys($values));
        $sql = "INSERT INTO {$this->qtableName} ({$fields}) VALUES ({$holders})";

        // 插入数据
        if (!$this->dbo->Execute($sql, $values, true)) {
            if ($unsetpk) { unset($row[$this->primaryKey]); }
            $this->dbo->completeTrans(false);
            return false;
        }

        // 如果提交的数据中没有主键字段值，则尝试获取新插入记录的主键值
        if (is_null($insertId)) {
            $insertId = $this->dbo->insertId();
            if (!$insertId) {
                if ($unsetpk) { unset($row[$this->primaryKey]); }
                $this->dbo->completeTrans(false);
                FLEA::loadClass('FLEA_Db_Exception_InvalidInsertID');
                return __THROW(new FLEA_Db_Exception_InvalidInsertID());
            }
        }

        // 处理关联数据表
        if ($this->autoLink && $saveLinks) {
            foreach (array_keys($this->links) as $linkKey) {
                $link =& $this->links[$linkKey];
                /* @var $link FLEA_Db_TableLink */
                if (!$link->enabled || !$link->linkCreate || !isset($row[$link->mappingName]) || !is_array($row[$link->mappingName])) {
                    // 跳过没有关联数据的关联和不需要处理的关联
                    continue;
                }

                if (!$link->saveAssocData($row[$link->mappingName], $insertId)) {
                    if ($unsetpk) { unset($row[$this->primaryKey]); }
                    $this->dbo->completeTrans(false);
                    return false;
                }
            }
        }

        $row[$this->primaryKey] = $insertId;
        $this->_updateCounterCache($row);

        // 提交事务
        $this->dbo->CompleteTrans();

        $this->_afterCreateDb($row);
        if ($unsetpk) { unset($row[$this->primaryKey]); }

        return $insertId;
    }

    /**
     * 插入多行记录，返回包含所有新记录主键值的数组，如果失败则返回 false
     *
     * @param array $rowset
     * @param boolean $saveLinks
     *
     * @return array
     */
    function createRowset(& $rowset, $saveLinks = true)
    {
        $insertids = array();
        $this->dbo->startTrans();
        foreach ($rowset as $row) {
            $insertid = $this->create($row, $saveLinks, false);
            if (!$insertid) {
                $this->dbo->completeTrans(false);
                return false;
            }
            $insertids[] = $insertid;
        }
        $this->dbo->completeTrans();
        return $insertids;
    }

    /**
     * 删除记录
     *
     * remove() 操作会引发 _beforeRemove()、_beforeRemoveDbByPkv()、_afterRemoveDbByPkv 和 _afterRemoveDb() 事件。
     *
     * @param array $row
     *
     * @return boolean
     */
    function remove(& $row, $removeLink = true)
    {
        if (!$this->_beforeRemove($row)) {
            return false;
        }

        if (!isset($row[$this->primaryKey])) {
            FLEA::loadClass('FLEA_Db_Exception_MissingPrimaryKey');
            __THROW(new FLEA_Db_Exception_MissingPrimaryKey($this->primaryKey));
            return false;
        }
        $ret = $this->removeByPkv($row[$this->primaryKey], $removeLink);
        if ($ret) {
            $this->_afterRemoveDb($row);
        }
        return $ret;
    }

    /**
     * 根据主键值删除记录
     *
     * removeByPkv() 引发 _beforeRemoveDbByPkv() 和 _afterRemoveDbByPkv() 事件。
     *
     * @param mixed $pkv
     * @param boolean $removeLink
     *
     * @return boolean
     */
    function removeByPkv($pkv, $removeLink = true)
    {
        $this->dbo->startTrans();

        if (!$this->_beforeRemoveDbByPkv($pkv)) {
            $this->dbo->completeTrans(false);
            return false;
        }

        /**
         * 首先删除关联表数据，再删除主表数据
         */
        $qpkv = $this->dbo->qstr($pkv);

        // 处理关联数据表
        $counterCacheLinks = array();
        if ($this->autoLink && $removeLink) {
            foreach (array_keys($this->links) as $linkKey) {
                $link =& $this->links[$linkKey];
                /* @var $link FLEA_Db_TableLink */
                if (!$link->enabled) { continue; }
                switch ($link->type) {
                case MANY_TO_MANY:
                    /* @var $link FLEA_Db_ManyToManyLink */
                    if (!$link->deleteMiddleTableDataByMainForeignKey($qpkv)) {
                        $this->dbo->completeTrans(false);
                        return false;
                    }
                    break;
                case HAS_ONE:
                case HAS_MANY:
                    /**
                     * 对于 HAS_ONE 和 HAS_MANY 关联，分为两种情况处理
                     *
                     * 当 $link->linkRemove 为 true 时，直接删除关联表中的关联数据
                     * 否则更新关联数据的外键值为 $link->linkRemoveFillValue
                     */
                    /* @var $link FLEA_Db_HasOneLink */
                    if ($link->deleteByForeignKey($qpkv) === false) {
                        $this->dbo->completeTrans(false);
                        return false;
                    }
                    break;
                case BELONGS_TO:
                    if ($link->counterCache) {
                        $counterCacheLinks[] = $link->foreignKey;
                    }
                }
            }
        }

        if (!empty($counterCacheLinks)) {
            $counterCacheLinks[] = $this->primaryKey;
            $row = $this->find(array($this->primaryKey => $pkv), null, $counterCacheLinks, false);
        }

        // 删除主表数据
        $sql = "DELETE FROM {$this->qtableName} WHERE {$this->qpk} = {$qpkv}";
        if ($this->dbo->execute($sql) == false) {
            $this->dbo->completeTrans(false);
            return false;
        }

        if (!empty($counterCacheLinks)) {
            $this->_updateCounterCache($row);
        }

        // 提交事务
        $this->dbo->completeTrans();

        $this->_afterRemoveDbByPkv($pkv);

        return true;
    }

    /**
     * 删除符合条件的记录
     *
     * @param mixed $conditions
     * @param boolean $removeLink
     *
     * @return boolean
     */
    function removeByConditions($conditions, $removeLink = true)
    {
        $rowset = $this->findAll($conditions, null, null, $this->primaryKey, false);
        $count = 0;
        $this->dbo->startTrans();
        foreach ($rowset as $row) {
            if (!$this->removeByPkv($row[$this->primaryKey], $removeLink)) { break; }
            $count++;
        }
        $this->dbo->completeTrans();
        $rows = $this->dbo->affectedRows();
        if ($rows > 0) { return $count; }
        return 0;
    }

    /**
     * 删除数组中所有主键值的记录，该操作不会处理关联
     *
     * @param array $pkvs
     * @param boolean $removeLink
     *
     * @return boolean
     */
    function removeByPkvs($pkvs, $removeLink = true)
    {
        $ret = true;
        $this->dbo->startTrans();
        foreach ($pkvs as $id) {
            $ret = $this->removeByPkv($id, $removeLink);
            if ($ret === false) { break; }
        }
        $this->dbo->completeTrans();
        return $ret;
    }

    /**
     * 删除所有记录
     *
     * @return boolean
     */
    function removeAll()
    {
        $sql = "DELETE FROM {$this->qtableName}";
        $ret = $this->execute($sql);
        return $ret;
    }

    /**
     * 删除所有记录及关联的数据
     *
     * @return boolean
     */
    function removeAllWithLinks()
    {
        $this->dbo->startTrans();

        // 处理关联数据表
        if ($this->autoLink) {
            foreach (array_keys($this->links) as $linkKey) {
                $link =& $this->links[$linkKey];
                /* @var $link FLEA_Db_TableLink */
                switch ($link->type) {
                case MANY_TO_MANY:
                    /* @var $link FLEA_Db_ManyToManyLink */
                    $link->init();
                    $sql = "DELETE FROM {$link->qjoinTable}";
                    break;
                case HAS_ONE:
                case HAS_MANY:
                    $link->init();
                    $sql = "DELETE FROM {$link->assocTDG->qtableName}";
                    break;
                default:
                    continue;
                }
                if ($this->dbo->execute($sql) == false) {
                    $this->dbo->completeTrans(false);
                    return false;
                }
            }
        }

        $sql = "DELETE FROM {$this->qtableName}";
        if ($this->dbo->execute($sql) == false) {
            $this->dbo->completeTrans(false);
            return false;
        }

        // 提交事务
        $this->dbo->completeTrans();

        return true;
    }

    /**
     * 启用所有或多个关联
     *
     * @param string|array $links
     */
    function enableLinks($links = null)
    {
        $this->autoLink = true;
        if (is_null($links)) {
            $links = array_keys($this->links);
        } elseif (!is_array($links)) {
            $links = explode(',', $links);
            $links = array_filter(array_map('trim', $links), 'strlen');
        }

        foreach ($links as $name) {
            $name = strtoupper($name);
            if (isset($this->links[$name])) {
                $this->links[$name]->enabled = true;
            }
        }
    }

    /**
     * 启用指定关联
     *
     * @param string $linkName
     *
     * @return FLEA_Db_TableLink
     *
     */
    function enableLink($linkName)
    {
        $link =& $this->getLink($linkName);
        if ($link) { $link->enabled = true; }
        $this->autoLink = true;
        return $link;
    }

    /**
     * 禁用所有或多个关联
     *
     * @param string|array $links
     */
    function disableLinks($links = null)
    {
        if (is_null($links)) {
            $links = array_keys($this->links);
            $this->autoLink = false;
        } elseif (!is_array($links)) {
            $links = explode(',', $links);
            $links = array_filter(array_map('trim', $links), 'strlen');
        }

        foreach ($links as $name) {
            $name = strtoupper($name);
            if (isset($this->links[$name])) {
                $this->links[$name]->enabled = false;
            }
        }
    }

    /**
     * 禁用指定关联
     *
     * @param string $linkName
     *
     * @return FLEA_Db_TableLink
     */
    function disableLink($linkName)
    {
        $link =& $this->getLink($linkName);
        if ($link) { $link->enabled = false; }
        return $link;
    }

    /**
     * 清除所有关联
     */
    function clearLinks()
    {
        $this->links = array();
    }

    /**
     * 根据类定义的 $hasOne、$hasMany、$belongsTo 和 $manyToMany 成员变量重建所有关联
     */
    function relink()
    {
        $this->clearLinks();
        $this->createLink($this->hasOne,     HAS_ONE);
        $this->createLink($this->belongsTo,  BELONGS_TO);
        $this->createLink($this->hasMany,    HAS_MANY);
        $this->createLink($this->manyToMany, MANY_TO_MANY);
    }

    /**
     * 获取指定名字的关联
     *
     * @param string $linkName
     *
     * @return FLEA_Db_TableLink
     */
    function & getLink($linkName)
    {
        $linkName = strtoupper($linkName);
        if (isset($this->links[$linkName])) {
            return $this->links[$linkName];
        }

        FLEA::loadClass('FLEA_Db_Exception_MissingLink');
        __THROW(new FLEA_Db_Exception_MissingLink($linkName));
        $ret = false;
        return $ret;
    }

    /**
     * 返回指定连接对应的表数据入口对象
     *
     * @param string $linkName
     *
     * @return FLEA_Db_TableDataGateway
     */
    function & getLinkTable($linkName)
    {
        $link =& $this->getLink($linkName);
        $link->init();
        return $link->assocTDG;
    }

    /**
     * 检查指定名字的关联是否存在
     *
     * @param string $name
     *
     * @return boolean
     */
    function existsLink($name)
    {
        return isset($this->links[strtoupper($name)]);
    }

    /**
     * 建立关联，并且返回新建立的关联对象
     *
     * @param array $defines
     * @param enum $type
     *
     * @return FLEA_Db_TableLink
     */
    function createLink($defines, $type)
    {
        if (!is_array($defines)) { return; }
        if (!is_array(reset($defines))) {
            $defines = array($defines);
        }

        // 创建关联对象
        foreach ($defines as $define) {
            if (!is_array($define)) { continue; }
            // 构造连接对象实例
            $link =& FLEA_Db_TableLink::createLink($define, $type, $this);
            $this->links[strtoupper($link->name)] =& $link;
        }
    }

    /**
     * 删除指定的关联
     *
     * @param string $linkName
     */
    function removeLink($linkName)
    {
        $linkName = strtoupper($linkName);
        if (isset($this->links[$linkName])) {
            unset($this->links[$linkName]);
        }
    }

    /**
     * 对数据进行验证
     *
     * 派生类可以覆盖此方法，以便进行附加的验证。
     *
     * @param array $row
     * @param int $skip
     *
     * @return boolean
     */
    function checkRowData(& $row, $skip = 0) {
        if (is_null($this->verifier)) { return false; }
        $this->lastValidationResult = $this->verifier->checkAll($row, $this->meta, $skip);
        return empty($this->lastValidationResult);
    }

    /**
     * 返回最后一次数据验证的结果
     *
     * @param string $info
     *
     * @return mixed
     */
    function getLastValidation($info = null) {
        if (is_null($info)) { return $this->lastValidationResult; }

        $arr = array();
        foreach ($this->lastValidationResult as $field => $check) {
            if (empty($check['rule'][$info])) {
                $arr[] = $field;
            } else {
                $arr[] = $check['rule'][$info];
            }
        }
        return $arr;
    }

    /**
     * 返回当前数据表的下一个插入 ID
     *
     * @return mixed
     */
    function newInsertId() {
        return $this->dbo->nextId($this->fullTableName . '_seq');
    }

    /**
     * 直接执行一个 sql 语句
     *
     * @param string $sql
     * @param array $inputarr
     *
     * @return mixed
     */
    function execute($sql, $inputarr = false)
    {
        return $this->dbo->execute($sql, $inputarr);
    }

    /**
     * 将 SQL 语句中的 ? 替换为相应的参数值
     *
     * @param string $sql
     * @param array $params
     *
     * @return string
     */
    function qinto($sql, $params = null)
    {
        if (!is_array($params)) {
            FLEA::loadClass('FLEA_Exception_TypeMismatch');
            return __THROW(new FLEA_Exception_TypeMismatch('$params', 'array', gettype($params)));
        }
        $arr = explode('?', $sql);
        $sql = array_shift($arr);
        foreach ($params as $value) {
            $sql .= $this->dbo->qstr($value) . array_shift($arr);
        }
        return $sql;
    }

    /**
     * 分析查询条件和参数
     *
     * 模式1：
     * where('user_id = ?', array($user_id))
     * where('user_id = :user_id', array('user_id' => $user_id))
     * where('user_id in (?)', array(array($id1, $id2, $id3)))
     *
     * 模式2：
     * where(array(
     *      'user_id' => $user_id,
     *      'level_ix' => $level_ix,
     * ))
     *
     * @param array|string $where
     * @param array $args
     *
     * @return array|string
     */
    function parseWhere($where, $args = null)
    {
        if (!is_array($args)) {
            $args = array();
        }
        if (is_array($where)) {
            return $this->_parseWhereArray($where);
        } else {
            return $this->_parseWhereString($where, $args);
        }
    }

    /**
     * 按照模式2对查询条件进行分析
     *
     * @param array $where
     *
     * @return array|string
     */
    function _parseWhereArray($where)
    {
        /**
         * 模式2：
         * where(array('user_id' => $user_id))
         * where(array('user_id' => $user_id, 'level_ix' => 1))
         * where(array('(', 'user_id' => $user_id, 'OR', 'level_ix' => $level_ix, ')'))
         * where(array('user_id' => array($id1, $id2, $id3)))
         */

        $parts = array();
        $callback = array($this->dbo, 'qstr');
        $next_op = '';

        foreach ($where as $key => $value) {
            if (is_int($key)) {
                $parts[] = $value;
                if ($value == ')') {
                    $next_op = 'AND';
                } else {
                    $next_op = '';
                }
            } else {
                if ($next_op != '') {
                    $parts[] = $next_op;
                }
                $field = $this->_parseWhereQfield(array('', $key));
                if (is_array($value)) {
                    $value = array_map($callback, $value);
                    $parts[] = $field . ' IN (' . implode(',', $value) . ')';
                } else {
                    $value = $this->dbo->qstr($value);
                    $parts[] = $field . ' = ' . $value;
                }
                $next_op = 'AND';
            }
        }

        return implode(' ', $parts);
    }

    /**
     * 按照模式1对查询条件进行分析
     *
     * @param string $where
     * @param array $args
     *
     * @return array|string
     */
    function _parseWhereString($where, $args = null)
    {
        /**
         * 模式1：
         * where('user_id = ?', array($user_id))
         * where('user_id = :user_id', array('user_id' => $user_id))
         * where('user_id in (?)', array(array($id1, $id2, $id3)))
         * where('user_id = :user_id', array('user_id' => $user_id))
         * where('user_id IN (:users_id)', array('users_id' => array(1, 2, 3)))
         */

        // 首先从查询条件中提取出可以识别的字段名
        if (strpos($where, '[') !== false) {
            // 提取字段名
            $where = preg_replace_callback('/\[([a-z0-9_\-\.]+)\]/i', array($this, '_parseWhereQfield'), $where);
        }

        return $this->qinto($where, $args);
    }

    /**
     * 将字段名替换为转义后的完全限定名
     *
     * @param array $matches
     *
     * @return string
     */
    function _parseWhereQfield($matches)
    {
        $p = explode('.', $matches[1]);
        switch (count($p)) {
        case 3:
            list($schema, $table, $field) = $p;
            if ($table == $this->tableName) {
                $table = $this->fullTableName;
            }
            return $this->dbo->qfield($field, $table, $schema);
        case 2:
            list($table, $field) = $p;
            if ($table == $this->tableName) {
                $table = $this->fullTableName;
            }
            return $this->dbo->qfield($field, $table);
        default:
            return $this->dbo->qfield($p[0]);
        }
    }

    /**
     * 返回转义后的数据
     *
     * @param mixed $value
     *
     * @return string
     */
    function qstr($value)
    {
        return $this->dbo->qstr($value);
    }

    /**
     * 获得一个字段名的完全限定名
     *
     * @param string $fieldName
     * @param string $tableName
     *
     * @return string
     */
    function qfield($fieldName, $tableName = null)
    {
        if (is_null($tableName)) {
            $tableName = $this->fullTableName;
        }
        return $this->dbo->qfield($fieldName, $tableName, $this->schema);
    }

    /**
     * 获得多个字段名的完全限定名
     *
     * @param string|array $fieldsName
     * @param string $tableName
     * @param boolean $returnArray
     *
     * @return string
     */
    function qfields($fieldsName, $tableName = null, $returnArray = false)
    {
        if (is_null($tableName)) {
            $tableName = $this->fullTableName;
        }
        return $this->dbo->qfields($fieldsName, $tableName, $this->schema, $returnArray);
    }

    /**
     * 分析查询条件，返回 WHERE 子句
     *
     * @param array $conditions
     * @param boolean $queryLinks
     *
     * @return string
     */
    function getWhere($conditions, $queryLinks = true) {
        // 处理查询条件
        $where = FLEA_Db_SqlHelper::parseConditions($conditions, $this);
        $sqljoin = '';
        $distinct = '';

        do {
            if (!is_array($where)) {
                $whereby = $where != '' ? " WHERE {$where}" : '';
                break;
            }

            $arr = $where;
            list($where, $linksWhere) = $arr;
            unset($arr);

            if (!$this->autoLink || !$queryLinks) {
                $whereby = $where != '' ? " WHERE {$where}" : '';
                break;
            }

            foreach ($linksWhere as $linkid => $lws) {
                if (!isset($this->links[$linkid]) || !$this->links[$linkid]->enabled) {
                    continue;
                }

                $link =& $this->links[$linkid];
                /* @var $link FLEA_Db_TableLink */
                if (!$link->init) { $link->init(); }
                $distinct = 'DISTINCT ';

                switch ($link->type) {
                case HAS_ONE:
                case HAS_MANY:
                    /* @var $link FLEA_Db_HasOneLink */
                    $sqljoin .= "LEFT JOIN {$link->assocTDG->qtableName} ON {$link->mainTDG->qpk} = {$link->qforeignKey} ";
                    break;
                case BELONGS_TO:
                    /* @var $link FLEA_Db_BelongsToLink */
                    $sqljoin .= "LEFT JOIN {$link->assocTDG->qtableName} ON {$link->assocTDG->qpk} = {$link->qforeignKey} ";
                    break;
                case MANY_TO_MANY:
                    /* @var $link FLEA_Db_ManyToManyLink */
                    $sqljoin .= "INNER JOIN {$link->qjoinTable} ON {$link->qforeignKey} = {$this->qpk} INNER JOIN {$link->assocTDG->qtableName} ON {$link->assocTDG->qpk} = {$link->qassocForeignKey} ";
                    break;
                }

                $lw = reset($lws);
                if (isset($lw[3])) {
                    $whereby = $where != '' ? " WHERE {$where} {$lw[3]} " : ' WHERE';
                } else {
                    $whereby = $where != '' ? " WHERE {$where} AND " : ' WHERE';
                }
                foreach ($lws as $lw) {
                    list($field, $value, $op, $expr, $isCommand) = $lw;
                    if (!$isCommand) {
                        $field = $link->assocTDG->qfield($field);
                        $value = $this->dbo->qstr($value);
                        $whereby .= " {$field} {$op} {$value} {$expr}";
                    } else {
                        $whereby .= " {$value} {$expr}";
                    }
                }
                $whereby = substr($whereby, 0, - (strlen($expr) + 1));

                unset($link);
            }

            $whereby = " {$sqljoin} {$whereby}";
        } while (false);

        if ($queryLinks) {
            return array($whereby, $distinct);
        } else {
            return $whereby;
        }
    }

    /**
     * 强制刷新缓存的数据表 meta 信息
     */
    function flushMeta()
    {
        $this->_prepareMeta(true);
    }

    /**
     * 更新记录的 updated 等字段
     *
     * @param array $row
     */
    function _setUpdatedTimeFields(& $row) {
        foreach ($this->updatedTimeFields as $af) {
            $af = strtoupper($af);
            if (!isset($this->meta[$af])) { continue; }
            switch ($this->meta[$af]['simpleType']) {
            case 'D': // 日期
            case 'T': // 日期时间
                // 由数据库驱动获取时间格式
                $row[$this->meta[$af]['name']] = $this->dbo->dbTimeStamp(time());
                break;
            case 'I': // Unix 时间戳
                $row[$this->meta[$af]['name']] = time();
                break;
            }
        }
    }

    /**
     * 更新记录的 created 和 updated 等字段
     *
     * @param array $row
     */
    function _setCreatedTimeFields(& $row) {
        $currentTime = time();
        $currentTimeStamp = $this->dbo->dbTimeStamp(time());
        foreach (array_merge($this->createdTimeFields, $this->updatedTimeFields) as $af) {
            $af = strtoupper($af);
            if (!isset($this->meta[$af])) { continue; }
            $afn = $this->meta[$af]['name'];
            if (!empty($row[$afn])) { continue; }

            switch ($this->meta[$af]['simpleType']) {
            case 'D': // 日期
            case 'T': // 日期时间
                // 由数据库驱动获取时间格式
                $row[$afn] = $currentTimeStamp;
                break;
            case 'I': // Unix 时间戳
                $row[$afn] = $currentTime;
                break;
            }
        }
    }

    /**
     * 准备当前数据表的元数据
     *
     * @param boolean $flushCache
     *
     * @return boolean
     */
    function _prepareMeta($flushCache = false) {
        $cached = FLEA::getAppInf('dbMetaCached');
        $cacheId = $this->dbo->dsn['id'] . '/' . $this->fullTableName;

        $readFromCache = ($cached != false && $flushCache == false);
        if ($readFromCache) {
            /**
             * 尝试从缓存读取
             */
            $meta = FLEA::getCache($cacheId, FLEA::getAppInf('dbMetaLifetime'));
            if (is_array($meta)) {
                $this->meta = $meta;
                return true;
            }
        }

        /**
         * 从数据库获得 meta
         */
        $this->meta = $this->dbo->metaColumns($this->qtableName);
        if (!is_array($this->meta) || empty($this->meta)) {
            FLEA::loadClass('FLEA_Db_Exception_MetaColumnsFailed');
            return __THROW(new FLEA_Db_Exception_MetaColumnsFailed($this->qtableName));
        }

        if ($cached) {
            return FLEA::writeCache($cacheId, $this->meta);
        } else {
            return true;
        }
    }

    /**
     * 调用 create() 方法后立即引发 _beforeCreate 事件
     *
     * 如果要阻止 create() 创建记录，该方法应该返回 false，否则返回 true。
     *
     * @param array $row
     *
     * @return boolean
     */
    function _beforeCreate(& $row)
    {
        return true;
    }

    /**
     * 调用 create() 方法后，表数据入口对数据进行处理后存入数据库前引发 _beforeCreateDb 事件
     *
     * 如果要阻止 create() 创建记录，该方法应该返回 false，否则返回 true。
     *
     * @param array $row
     *
     * @return boolean
     */
    function _beforeCreateDb(& $row)
    {
        return true;
    }

    /**
     * 调用 create() 方法并且成功将数据存入数据库后引发 _afterCreateDb 事件
     *
     * @param array $row
     */
    function _afterCreateDb(& $row)
    {
    }


    /**
     * 调用 update() 方法后立即引发 _beforeUpdate 事件
     *
     * 如果要阻止 update() 更新记录，该方法应该返回 false，否则返回 true。
     *
     * @param array $row
     *
     * @return boolean
     */
    function _beforeUpdate(& $row)
    {
        return true;
    }

    /**
     * 调用 update() 方法后，表数据入口对数据进行处理后存入数据库前引发 _beforeUpdateDb 事件
     *
     * 如果要阻止 update() 更新记录，该方法应该返回 false，否则返回 true。
     *
     * @param array $row
     *
     * @return boolean
     */
    function _beforeUpdateDb(& $row)
    {
        return true;
    }

    /**
     * 调用 update() 方法并且成功将数据更新到数据库后引发 _afterUpdateDb 事件
     *
     * @param array $row
     */
    function _afterUpdateDb(& $row)
    {
    }

    /**
     * 调用 remove() 方法后立即引发 _beforeRemove 事件
     *
     * 如果要阻止 remove() 删除记录，该方法应该返回 false，否则返回 true。
     *
     * @param array $row
     *
     * @return boolean
     */
    function _beforeRemove(& $row)
    {
        return true;
    }

    /**
     * 调用 remove() 方法并且成功删除记录后引发 _afterRemoveDb 事件
     *
     * @param array $row
     */
    function _afterRemoveDb($row)
    {
    }

    /**
     * 调用 remove() 或 removeByPkv() 方法后立即引发 _beforeRemoveDbByPkv 事件
     *
     * 调用 remove() 方法时，_beforeRemoveDbByPkv 事件出现在 _beforeRemove 事件之后。
     *
     * 如果要阻止 remove() 或 removeByPkv() 删除记录，
     * 该方法应该返回 false，否则返回 true。
     *
     * @param mixed $pkv
     *
     * @return boolean
     */
    function _beforeRemoveDbByPkv($pkv)
    {
        return true;
    }

    /**
     * 调用 remove() 或 removeByPkv() 方法并且成功删除记录后引发 _afterRemoveDbByPkv 事件
     *
     * @param array $row
     */
    function _afterRemoveDbByPkv($pkv)
    {
    }

    /**
     * 根据关联表的 counterCache 选项更新统计信息
     *
     * @param array $row
     */
    function _updateCounterCache(& $row)
    {
        foreach (array_keys($this->links) as $linkKey) {
            $link =& $this->links[$linkKey];
            /* @var $link FLEA_Db_TableLink */
            if ($link->type != BELONGS_TO || !$link->enabled || !$link->counterCache) { continue; }
            $link->init();
            $f = $link->assocTDG->qfield($link->counterCache);
            if (isset($row[$link->foreignKey])) {
                $fkv = $this->dbo->qstr($row[$link->foreignKey]);
            } else {
                $pkv = $this->dbo->qstr($row[$this->primaryKey]);
                $sql = "SELECT {$link->foreignKey} FROM {$this->qtableName} WHERE {$this->qpk} = {$pkv}";
                $fkv = $this->dbo->getOne($sql);
            }

            $conditions = "{$link->qforeignKey} = {$fkv}";
            if ($link->conditions) {
                if (is_array($link->conditions)) {
                    $conditions = FLEA_Db_SqlHelper::parseConditions($link->conditions, $link->assocTDG);
                    if (is_array($conditions)) {
                        $conditions = $conditions[0];
                    }
                } else {
                    $conditions =& $link->conditions;
                }
                if ($conditions) {
                    $conditions = "{$link->qforeignKey} = {$fkv} AND {$conditions}";
                }
            }

            $sql = "UPDATE {$link->assocTDG->qtableName} SET {$f} = (SELECT COUNT(*) FROM {$this->qtableName} WHERE {$conditions}) WHERE {$link->assocTDG->qpk} = {$fkv}";
            $this->dbo->execute($sql);
        }
    }
}
