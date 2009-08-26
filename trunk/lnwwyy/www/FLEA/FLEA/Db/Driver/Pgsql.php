<?php
/////////////////////////////////////////////////////////////////////////////
// FleaPHP Framework
//
// 该文件由“夜猫子”共享，特此感谢！
/////////////////////////////////////////////////////////////////////////////

/**
 * 定义 FLEA_Db_Driver_Pgsql 驱动
 *
 * 由夜猫子提供，廖宇雷修改，并参考 AdoDB 的 MetaColumns() 及 SelectLimit() 方法。
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 夜猫子 yangyi.cn.gz #AT# gmail.com
 * @package Core
 * @version $Id: Pgsql.php 984 2007-10-19 09:44:53Z qeeyuan $
 */

/**
 * 用于 pgsql 扩展的数据库驱动程序
 *
 * @package Core
 * @author 夜猫子 yangyi.cn.gz #AT# gmail.com
 * @version 1.1
 */
class FLEA_Db_Driver_Pgsql
{
    /**
     * 用于 genSeq()、dropSeq() 和 nextId() 的 SQL 查询语句
     */
    var $NEXT_ID_SQL    = "SELECT NEXTVAL('%s')";
    var $CREATE_SEQ_SQL = "CREATE SEQUENCE %s START %s";
    var $DROP_SEQ_SQL   = "DROP SEQUENCE %s";

    /**
     * 用于描绘 true、false 和 null 的数据库值
     */
    var $TRUE_VALUE  = 1;
    var $FALSE_VALUE = 0;
    var $NULL_VALUE = 'NULL';

    /**
     * 用于获取元数据的 SQL 查询语句
     */
    var $META_COLUMNS_SQL = "SELECT a.attname,t.typname,a.attlen,a.atttypmod,a.attnotnull,a.atthasdef,a.attnum FROM pg_class c, pg_attribute a,pg_type t WHERE relkind in ('r','v') AND (c.relname='%s' or c.relname = lower('%s')) and a.attname not like '....%%' AND a.attnum > 0 AND a.atttypid = t.oid AND a.attrelid = c.oid ORDER BY a.attnum";

    // 用于获取元数据的 SQL 查询语句（定义了 Schema 时使用）
    var $META_COLUMNS_SQL1 = "SELECT a.attname, t.typname, a.attlen, a.atttypmod, a.attnotnull, a.atthasdef, a.attnum FROM pg_class c, pg_attribute a, pg_type t, pg_namespace n WHERE relkind in ('r','v') AND (c.relname='%s' or c.relname = lower('%s')) and c.relnamespace=n.oid and n.nspname='%s' and a.attname not like '....%%' AND a.attnum > 0 AND a.atttypid = t.oid AND a.attrelid = c.oid ORDER BY a.attnum";

    // 获取主键字段的查询语句
    var $META_KEY_SQL = "SELECT ic.relname AS index_name, a.attname AS column_name,i.indisunique AS unique_key, i.indisprimary AS primary_key FROM pg_class bc, pg_class ic, pg_index i, pg_attribute a WHERE bc.oid = i.indrelid AND ic.oid = i.indexrelid AND (i.indkey[0] = a.attnum OR i.indkey[1] = a.attnum OR i.indkey[2] = a.attnum OR i.indkey[3] = a.attnum OR i.indkey[4] = a.attnum OR i.indkey[5] = a.attnum OR i.indkey[6] = a.attnum OR i.indkey[7] = a.attnum) AND a.attrelid = bc.oid AND bc.relname = '%s'";

    // 获取字段默认值的查询语句
    var $META_DEFAULT_SQL = "SELECT d.adnum as num, d.adsrc as def from pg_attrdef d, pg_class c where d.adrelid=c.oid and c.relname='%s' order by d.adnum";

    /**
     * 数据库连接信息
     *
     * @var array
     */
    var $dsn = null;

    /**
     * 数据库连接句柄
     *
     * @var resource
     */
    var $conn = null;

    /**
     * 所有 SQL 查询的日志
     *
     * @var array
     */
    var $log = array();

    /**
     * 指示是否记录 SQL 语句（部署模式时该设置默认为 false）
     *
     * @var boolean
     */
    var $enableLog = false;

    /**
     * 最后一次数据库操作的错误信息
     *
     * @var mixed
     */
    var $lasterr = null;

    /**
     * 最后一次数据库操作的错误代码
     *
     * @var mixed
     */
    var $lasterrcode = null;

    /**
     * 指示事务启动次数
     *
     * @var int
     */
    var $_transCount = 0;

    /**
     * 指示事务是否提交
     *
     * @var boolean
     */
    var $_transCommit = true;

    /**
     * 最后一次查询的结果
     *
     * @var mixed
     */
    var $_lastrs = null;

    /**
     * 构造函数
     *
     * @param array $dsn
     */
    function FLEA_Db_Driver_Pgsql($dsn = false)
    {
        $tmp = (array)$dsn;
        unset($tmp['password']);
        $this->dsn = $dsn;
        $this->enableLog = !defined('DEPLOY_MODE') || DEPLOY_MODE != true;
        if (!function_exists('log_message')) {
            $this->enableLog = false;
        }
    }

    /**
     * 连接数据库
     *
     * @param array $dsn
     *
     * @return boolean
     */
    function connect($dsn = false)
    {
        $this->lasterr = null;
        $this->lasterrcode = null;

        if ($this->conn && $dsn == false) { return true; }
        if (!$dsn) {
            $dsn = $this->dsn;
        } else {
            $this->dsn = $dsn;
        }
        $dsnstring = '';
        if (isset($dsn['host'])) {
            $dsnstring = 'host=' . $this->_addslashes($dsn['host']);
        }
        if (isset($dsn['port'])) {
            $dsnstring .= ' port=' . $this->_addslashes($dsn['port']);
        }
        if (isset($dsn['login'])) {
            $dsnstring .= ' user=' . $this->_addslashes($dsn['login']);
        }
        if (isset($dsn['password'])) {
            $dsnstring .= ' password=' . $this->_addslashes($dsn['password']);
        }
        if (isset($dsn['database'])) {
            $dsnstring .= ' dbname=' . $this->_addslashes($dsn['database']);
        }
        $dsnstring .= ' ';

        if (isset($dsn['options'])) {
            $this->conn = pg_connect($dsnstring, $dsn['options']);
        } else {
            $this->conn = pg_connect($dsnstring);
        }

        if (!$this->conn) {
            FLEA::loadClass('FLEA_Db_Exception_SqlQuery');
            $pos = strpos($dsnstring, 'password=');
            if ($pos !== false) {
                $dsnstring = substr($dsnstring, 0, $pos - 1) . substr($dsnstring, strpos($dsnstring, ' ', $pos + 1));
            }
            __THROW(new FLEA_Db_Exception_SqlQuery("pg_connect(\"{$dsnstring}\") failed!"));
            return false;
        }

        if (!$this->execute("set datestyle='ISO'")) { return false; }

        if (isset($dsn['charset']) && $dsn['charset'] != '') {
            $charset = $dsn['charset'];
        } else {
            $charset = FLEA::getAppInf('databaseCharset');
        }
        if (strtoupper($charset) == 'GB2312') { $charset = 'GBK'; }
        if ($charset != '') {
            pg_set_client_encoding($this->conn, $charset);
        }

        return true;
    }

    /**
     * 关闭数据库连接
     */
    function close()
    {
        if ($this->conn) {
            pg_close($this->conn);
        }
        $this->conn = null;
        $this->lasterr = null;
        $this->lasterrcode = null;
        $this->_transCount = 0;
        $this->_transCommit = true;
    }

    /**
     * 执行一个查询，返回一个 resource 或者 boolean 值
     *
     * @param string $sql
     * @param array $inputarr
     * @param boolean $throw 指示查询出错时是否抛出异常
     *
     * @return resource|boolean
     */
    function execute($sql, $inputarr = null, $throw = true)
    {
        if (is_array($inputarr)) {
            $sql = $this->_prepareSql($sql, $inputarr);
        }
        if ($this->enableLog) {
            $this->log[] = $sql;
            log_message("sql: {$sql}", 'debug');
        }
        $this->_lastrs = @pg_exec($this->conn, $sql);
        if ($this->_lastrs !== false) {
            $this->lasterr = null;
            $this->lasterrcode = null;
            return $this->_lastrs;
        }
        $this->lasterr = pg_errormessage($this->conn);
        $this->lasterrcode = null;
        if (!$throw) { return false; }

        FLEA::loadClass('FLEA_Db_Exception_SqlQuery');
        __THROW(new FLEA_Db_Exception_SqlQuery($sql, $this->lasterr));
        return false;
    }

    /**
     * 转义字符串
     *
     * @param string $value
     *
     * @return mixed
     */
    function qstr($value)
    {
        if (is_bool($value)) { return $value ? $this->TRUE_VALUE : $this->FALSE_VALUE; }
        if (is_null($value)) { return $this->NULL_VALUE; }
        return "'" . pg_escape_string($value) . "'";
    }

    /**
     * 将数据表名字转换为完全限定名
     *
     * @param string $tableName
     *
     * @return string
     */
    function qtable($tableName)
    {
        if (substr($tableName, 0, 1) == '"') { return $tableName; }
        return '"' . $tableName . '"';
    }

    /**
     * 将字段名转换为完全限定名，避免因为字段名和数据库关键词相同导致的错误
     *
     * @param string $fieldName
     * @param string $tableName
     *
     * @return string
     */
    function qfield($fieldName, $tableName = null)
    {
        $pos = strpos($fieldName, '.');
        if ($pos !== false) {
            $tableName = substr($fieldName, 0, $pos);
            $fieldName = substr($fieldName, $pos + 1);
        }
        if ($tableName != "") {
            return "\"{$tableName}\".\"{$fieldName}\"";
        } else {
            return "\"{$fieldName}\"";
        }
    }

    /**
     * 一次性将多个字段名转换为完全限定名
     *
     * @param string|array $fields
     * @param string $tableName
     *
     * @return string
     */
    function qfields($fields, $tableName = null)
    {
        if (!is_array($fields)) {
            $fields = explode(',', $fields);
        }
        $return = array();
        foreach ($fields as $fieldName) {
            $fieldName = trim($fieldName);
            if ($fieldName == '') { continue; }
            $pos = strpos($fieldName, '.');
            if ($pos !== false) {
                $tableName = substr($fieldName, 0, $pos);
                $fieldName = substr($fieldName, $pos + 1);
            }
            if ($tableName != '') {
                $return[] = "\"{$tableName}\".\"{$fieldName}\"";
            } else {
                $return[] = "\"{$fieldName}\"";
            }
        }
        return implode(', ', $return);
    }

    /**
     * 为数据表产生下一个序列值，失败返回 false
     *
     * @param string $seqName
     * @param string $startValue
     *
     * @return int
     */
    function nextId($seqName = 'sdbo_seq', $startValue = 1)
    {
        $getNextId = sprintf($this->NEXT_ID_SQL, $seqName);
        $result = $this->execute($getNextId, null, false);
        if ($result == false) {
            // 序列不存在，建立该序列
            if (!$this->createSeq($seqName, $startValue)) { return false; }
            if (!$result = $this->execute($getNextId)) { return false; }
        }

        $row = $this->fetchRow($result);
        $this->freeRes($result);
        return reset($row);
    }

    /**
     * 创建一个新的序列，成功返回 true，失败返回 false
     *
     * @param string $seqName
     * @param int $startValue
     *
     * @return boolean
     */
    function createSeq($seqName = 'sdbo_seq', $startValue = 1)
    {
        return $this->execute(sprintf($this->CREATE_SEQ_SQL, $seqName, $startValue));
    }

    /**
     * 删除一个序列，成功返回 true，失败返回 false
     *
     * @param string $seqName
     */
    function dropSeq($seqName = 'sdbo_seq')
    {
        return $this->execute(sprintf($this->DROP_SEQ_SQL, $seqName));
    }

    /**
     * 获取自增字段的最后一个值
     *
     * 如果没有可返回的值，则抛出异常。
     *
     * @return mixed
     */
    function insertId()
    {
        require_once(FLEA_DIR . '/Exception/NotImplemented.php');
        __THROW(new FLEA_Exception_NotImplemented('insertId()', 'FLEA_Db_Driver_Pgsql'));
        return false;
    }

    /**
     * 返回最近一次数据库操作受到影响的记录数
     *
     * @return int
     */
    function affectedRows()
    {
        return pg_affected_rows($this->_lastrs);
    }

    /**
     * 从记录集中返回一行数据
     *
     * @param resouce $res
     *
     * @return array
     */
    function fetchRow($res)
    {
        return pg_fetch_row($res);
    }

    /**
     * 从记录集中返回一行数据，字段名作为键名
     *
     * @param resouce $res
     *
     * @return array
     */
    function fetchAssoc($res)
    {
        return pg_fetch_assoc($res);
    }

    /**
     * 释放查询句柄
     *
     * @param resource $res
     */
    function freeRes($res)
    {
        pg_free_result($res);
    }

    /**
     * 进行限定记录集的查询（PostgreSQL 7 以上版本可用）
     *
     * @param string $sql
     * @param int $length
     * @param int $offset
     */
    function selectLimit($sql, $length = 'ALL', $offset = 0)
    {
        if (strtoupper($length) != 'ALL') { $length = (int)$length; }
        $sql = sprintf('%s LIMIT %d OFFSET %d', $sql, $length, (int)$offset);
        return $this->execute($sql);
    }

    /**
     * 执行一个查询，返回查询结果记录集
     *
     * @param string|resource $sql
     *
     * @return array
     */
    function & getAll($sql)
    {
        if (is_resource($sql)) {
            $res = $sql;
        } else {
            $res = $this->execute($sql);
        }
        $data = array();
        while ($row = pg_fetch_assoc($res)) {
            $data[] = $row;
        }
        pg_free_result($res);
        return $data;
    }

    /**
     * 执行一个查询，返回分组后的查询结果记录集
     *
     * $groupBy 参数如果为字符串或整数，表示结果集根据 $groupBy 参数指定的字段进行分组。
     * 如果 $groupBy 参数为 true，则表示根据每行记录的第一个字段进行分组。
     *
     * @param string|resource $sql
     * @param string|int|boolean $groupBy
     *
     * @return array
     */
    function & getAllGroupBy($sql, & $groupBy)
    {
        if (is_resource($sql)) {
            $res = $sql;
        } else {
            $res = $this->execute($sql);
        }
        $data = array();
        $row = pg_fetch_assoc($res);
        if ($row != false) {
            if ($groupBy === true) {
                $groupBy = key($row);
            }
            do {
                $rkv = $row[$groupBy];
                unset($row[$groupBy]);
                $data[$rkv][] = $row;
            } while ($row = pg_fetch_assoc($res));
        }
        pg_free_result($res);
        return $data;
    }

    /**
     * 执行一个查询，返回查询结果记录集、指定字段的值集合以及以该字段值分组后的记录集
     *
     * @param string|resource $sql
     * @param string $field
     * @param array $fieldValues
     * @param array $reference
     *
     * @return array
     */
    function getAllWithFieldRefs($sql, $field, & $fieldValues, & $reference)
    {
        if (is_resource($sql)) {
            $res = $sql;
        } else {
            $res = $this->execute($sql);
        }

        $fieldValues = array();
        $reference = array();
        $offset = 0;
        $data = array();
        while ($row = pg_fetch_assoc($res)) {
            $fieldValue = $row[$field];
            unset($row[$field]);
            $data[$offset] = $row;
            $fieldValues[$offset] = $fieldValue;
            $reference[$fieldValue] =& $data[$offset];
            $offset++;
        }
        pg_free_result($res);
        return $data;
    }

    /**
     * 执行一个查询，并将数据按照指定字段分组后与 $assocRowset 记录集组装在一起
     *
     * @param string|resource $sql
     * @param array $assocRowset
     * @param string $mappingName
     * @param boolean $oneToOne
     * @param string $refKeyName
     * @param mixed $limit
     */
    function assemble($sql, & $assocRowset, $mappingName, $oneToOne, $refKeyName, $limit = null)
    {
        if (is_resource($sql)) {
            $res = $sql;
        } else {
            if (!is_null($limit)) {
                if (is_array($limit)) {
                    list($length, $offset) = $limit;
                } else {
                    $length = $limit;
                    $offset = 0;
                }
                $res = $this->selectLimit($sql, $length, $offset);
            } else {
                $res = $this->execute($sql);
            }
        }
        if ($oneToOne) {
            // 一对一组装数据
            while ($row = pg_fetch_assoc($res)) {
                $rkv = $row[$refKeyName];
                unset($row[$refKeyName]);
                $assocRowset[$rkv][$mappingName] = $row;
            }
        } else {
            // 一对多组装数据，需要检查是否有全 NULL 的记录
            while ($row = pg_fetch_assoc($res)) {
                $rkv = $row[$refKeyName];
                unset($row[$refKeyName]);
                $assocRowset[$rkv][$mappingName][] = $row;
            }
        }

        pg_free_result($res);
    }

    /**
     * 执行查询，返回第一条记录的第一个字段
     *
     * @param string $sql
     *
     * @return mixed
     */
    function getOne($sql)
    {
        if (is_resource($sql)) {
            $res = $sql;
        } else {
            $res = $this->execute($sql);
        }
        $row = pg_fetch_row($res);
        pg_free_result($res);
        return isset($row[0]) ? $row[0] : null;
    }

    /**
     * 执行查询，返回第一条记录
     *
     * @param string $sql
     *
     * @return mixed
     */
    function & getRow($sql)
    {
        if (is_resource($sql)) {
            $res = $sql;
        } else {
            $res = $this->execute($sql);
        }
        $row = pg_fetch_assoc($res);
        pg_free_result($res);
        return $row;
    }

    /**
     * 执行查询，返回结果集的第一列
     *
     * @param string|resource $sql
     * @param int $col 要返回的列，0 为第一列
     *
     * @return mixed
     */
    function & getCol($sql, $col = 0)
    {
        if (is_resource($sql)) {
            $res = $sql;
        } else {
            $res = $this->execute($sql);
        }
        $data = array();
        while ($row = pg_fetch_row($res)) {
            $data[] = $row[$col];
        }
        pg_free_result($res);
        return $data;
    }

    /**
     * 返回指定表（或者视图）的元数据
     *
     * 部分代码参考 ADOdb 实现。
     *
     * 每个字段包含下列属性：
     *
     * name:            字段名
     * scale:           小数位数
     * type:            字段类型
     * simpleType:      简单字段类型（与数据库无关）
     * maxLength:       最大长度
     * notNull:         是否不允许保存 NULL 值
     * primaryKey:      是否是主键
     * autoIncrement:   是否是自动增量字段
     * binary:          是否是二进制数据
     * unsigned:        是否是无符号数值
     * hasDefault:      是否有默认值
     * defaultValue:    默认值
     *
     * @param string $table
     * @param boolean $normalize 指示是否规格化字段名
     *
     * @return array
     */
    function & metaColumns($table, $normalize = true)
    {
        static $typeMap = array(
            'MONEY' => 'C',
            'INTERVAL' => 'C',
            'CHAR' => 'C',
            'CHARACTER' => 'C',
            'VARCHAR' => 'C',
            'NAME' => 'C',
            'BPCHAR' => 'C',
            '_VARCHAR' => 'C',
            'INET' => 'C',
            'MACADDR' => 'C',

            'TEXT' => 'X',
            'IMAGE' => 'B',
            'BLOB' => 'B',
            'BIT' => 'B',
            'VARBIT' => 'B',
            'BYTEA' => 'B',

            'BOOL' => 'L',
            'BOOLEAN' => 'L',

            'DATE' => 'D',

            'TIMESTAMP WITHOUT TIME ZONE' => 'T',
            'TIME' => 'T',
            'DATETIME' => 'T',
            'TIMESTAMP' => 'T',
            'TIMESTAMPTZ' => 'T',

            'SMALLINT' => 'I',
            'BIGINT' => 'I',
            'INTEGER' => 'I',
            'INT8' => 'I',
            'INT4' => 'I',
            'INT2' => 'I',

            'OID' => 'R',
            'SERIAL' => 'R',
        );

        $schema = false;
        $this->_findschema($table, $schema);

        if (!empty($this->META_KEY_SQL)) {
            // If we want the primary keys, we have to issue a separate query
            // Of course, a modified version of the metaColumnsSQL query using a
            // LEFT JOIN would have been much more elegant, but postgres does
            // not support OUTER JOINS. So here is the clumsy way.
            // fetch all result in once for performance.
            $keys = $this->getAll(sprintf($this->META_KEY_SQL, $table));
        }

        $rsdefa = array();
        if (!empty($this->META_DEFAULT_SQL)) {
            $sql = sprintf($this->META_DEFAULT_SQL, $table);
            $rsdef = $this->execute($sql);
            if ($rsdef) {
                while ($row = pg_fetch_assoc($rsdef)) {
                    $num = $row['num'];
                    $s = $row['def'];
                    if (strpos($s, '::') === false && strpos($s, "'") === 0) {
                        /* quoted strings hack... for now... fixme */
                        $s = substr($s, 1);
                        $s = substr($s, 0, strlen($s) - 1);
                    }

                    $rsdefa[$num] = $s;
                }
                pg_free_result($rsdef);
            }
            unset($rsdef);
        }

        if ($schema) {
            $rs = $this->execute(sprintf($this->META_COLUMNS_SQL1, $table, $table, $schema));
        } else {
            $rs = $this->execute(sprintf($this->META_COLUMNS_SQL, $table, $table));
        }
        if (!$rs) { return false; }

        $retarr = array();
        while (($row = pg_fetch_row($rs))) {
            $field = array();
            $field['name'] = $row[0];
            $field['type'] = $row[1];
            $field['maxLength'] = $row[2];
            $field['attnum'] = $row[6];

            if ($field['maxLength'] <= 0) {
                $field['maxLength'] = $row[3] - 4;
            }
            if ($field['maxLength'] <= 0) {
                $field['maxLength'] = -1;
            }

            if ($field['type'] == 'numeric') {
                $field['scale'] = $field['maxLength'] & 0xFFFF;
                $field['maxLength'] >>= 16;
            }
            // dannym
            // 5 hasdefault; 6 num-of-column
            $field['hasDefault'] = ($row[5] == 't');
            if ($field['hasDefault']) {
                $field['defaultValue'] = $rsdefa[$row[6]];
            }

            $field['notNull'] = $row[4] == 't';

            if (is_array($keys)) {
                foreach($keys as $key) {
                    if ($field['name'] == $key['column_name'] && $key['primary_key'] == 't') {
                        $field['primaryKey'] = true;
                    } else {
                        $field['primaryKey'] = false;
                    }
                    if ($field['name'] == $key['column_name'] && $key['unique_key'] == 't') {
                        $field['unique'] = true; // What name is more compatible?
                    } else {
                        $field['unique'] = false;
                    }
                }
            }

            $t = strtoupper($field['type']);
            if (isset($typeMap[$t])) {
                $field['simpleType'] = $typeMap[$t];
            } else {
                $field['simpleType'] = 'N';
            }

            if ($field['simpleType'] == 'I' && ($field['primaryKey'] != false || $field['unique'] != false)) {
                $field['simpleType'] = 'R';
            }

            $field['autoIncrement'] = false;

            if ($normalize) {
                $retarr[strtoupper($field['name'])] = $field;
            } else {
                $retarr[$field['name']] = $field;
            }
        }
        pg_free_result($rs);
        return $retarr;
    }

    /**
     * 返回数据库可以接受的日期格式
     *
     * @param int $timestamp
     */
    function dbTimeStamp($timestamp)
    {
        return date('Y-m-d H:i:s', $timestamp);
    }

    /**
     * 启动事务
     */
    function startTrans()
    {
        $this->_transCount += 1;
        if ($this->_transCount == 1) {
            $this->execute('BEGIN');
        }
    }

    /**
     * 完成事务，根据查询是否出错决定是提交事务还是回滚事务
     *
     * 如果 $commitOnNoErrors 参数为 true，当事务中所有查询都成功完成时，则提交事务，否则回滚事务
     * 如果 $commitOnNoErrors 参数为 false，则强制回滚事务
     *
     * @param $commitOnNoErrors 指示在没有错误时是否提交事务
     */
    function completeTrans($commitOnNoErrors = true)
    {
        if ($this->_transCount < 1) { return; }
        if ($this->_transCount > 1) {
            $this->_transCount -= 1;
            return;
        }
        $this->_transCount = 0;

        if ($this->_transCommit && $commitOnNoErrors) {
            $this->execute('COMMIT');
        } else {
            $this->execute('ROLLBACK');
        }
    }

    /**
     * 强制指示在调用 completeTrans() 时回滚事务
     */
    function failTrans()
    {
        $this->_transCommit = false;
    }

    /**
     * 反复事务是否失败的状态
     */
    function hasFailedTrans()
    {
        if ($this->_transCount > 0) {
            return $this->_transCommit === false;
        }
        return false;
    }

    /**
     * 根据 SQL 语句和提供的参数数组，生成最终的 SQL 语句
     *
     * @param string $sql
     * @param array $inputarr
     *
     * @return string
     */
    function _prepareSql($sql, & $inputarr)
    {
        $sqlarr = explode('?', $sql);
        $sql = '';
        $ix = 0;
        foreach ($inputarr as $v) {
            $sql .= $sqlarr[$ix];
            $typ = gettype($v);
            if ($typ == 'string') {
                $sql .= $this->qstr($v);
            } else if ($typ == 'double') {
                $sql .= $this->qstr(str_replace(',', '.', $v));
            } else if ($typ == 'boolean') {
                $sql .= $v ? $this->TRUE_VALUE : $this->FALSE_VALUE;
            } else if (is_null($v)) {
                $sql .= 'NULL';
            } else {
                $sql .= $v;
            }
            $ix += 1;
        }
        if (isset($sqlarr[$ix])) {
            $sql .= $sqlarr[$ix];
        }
        return $sql;
    }

    /**
     *  按照 PostgreSQL 的要求转义 DSN 字符串参数
     *
     * @param string $s
     *
     * @return string
     */
    function _addslashes($s)
    {
        $len = strlen($s);
        if ($len == 0) return "''";
        if (strncmp($s,"'",1) === 0 && substr($s,$len-1) == "'") return $s; // already quoted
        return "'".addslashes($s)."'";
    }

    /**
     * 分离数据库模式和数据表的名字
     *
     * @param string $table
     * @param string $schema
     */
    function _findschema(& $table, & $schema)
    {
        if (!$schema && ($at = strpos($table, '.')) !== false) {
            $schema = substr($table, 0, $at);
            $table = substr($table, $at + 1);
        }
    }
}
