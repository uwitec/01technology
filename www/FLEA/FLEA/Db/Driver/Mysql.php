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
 * 定义 FLEA_Db_Driver_Mysql 驱动
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Mysql.php 1077 2008-05-14 17:44:19Z dualface $
 */

// {{{ includes
FLEA::loadClass('FLEA_Db_Driver_Abstract');
// }}}

/**
 * 用于 mysql 扩展的数据库驱动程序
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.1
 */
class FLEA_Db_Driver_Mysql extends FLEA_Db_Driver_Abstract
{
    var $NEXT_ID_SQL    = 'UPDATE %s SET id = LAST_INSERT_ID(id + 1)';
    var $CREATE_SEQ_SQL = 'CREATE TABLE %s (id INT NOT NULL)';
    var $INIT_SEQ_SQL   = 'INSERT INTO %s VALUES (%s)';
    var $DROP_SEQ_SQL   = 'DROP TABLE %s';
    var $META_COLUMNS_SQL = 'SHOW FULL COLUMNS FROM %s';
    var $PARAM_STYLE = DBO_PARAM_QM;
    var $HAS_INSERT_ID  = true;
    var $HAS_AFFECTED_ROWS = true;
    var $_mysqlVersion = null;

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
        if (isset($dsn['port']) && $dsn['port'] != '') {
            $host = $dsn['host'] . ':' . $dsn['port'];
        } else {
            $host = $dsn['host'];
        }
        if (!isset($dsn['login'])) { $dsn['login'] = ''; }
        if (!isset($dsn['password'])) { $dsn['password'] = ''; }
        if (!empty($dsn['options'])) {
            $this->conn = mysql_connect($host, $dsn['login'], $dsn['password'], false, $dsn['options']);
        } else {
            $this->conn = mysql_connect($host, $dsn['login'], $dsn['password']);
        }

        if (!$this->conn) {
            FLEA::loadClass('FLEA_Db_Exception_SqlQuery');
            return __THROW(new FLEA_Db_Exception_SqlQuery("mysql_connect('{$host}', '{$dsn['login']}') failed!", mysql_error(), mysql_errno()));
        }

        if (!empty($dsn['database'])) {
            if (!$this->selectDb($dsn['database'])) { return false; }
        }

        $this->_mysqlVersion = $this->getOne('SELECT VERSION()');
        if (isset($dsn['charset']) && $dsn['charset'] != '') {
            $charset = $dsn['charset'];
        } else {
            $charset = FLEA::getAppInf('databaseCharset');
        }
        if ($this->_mysqlVersion >= '4.1' && $charset != '') {
            if (!$this->execute("SET NAMES '" . $charset . "'")) { return false; }
        }

        return true;
    }

    function close()
    {
        if ($this->conn) { mysql_close($this->conn); }
        parent::close();
    }

    function selectDb($database)
    {
        if (!mysql_select_db($database, $this->conn)) {
            FLEA::loadClass('FLEA_Db_Exception_SqlQuery');
            return __THROW(new FLEA_Db_Exception_SqlQuery("SELECT DATABASE: '{$database}' FAILED!", mysql_error($this->conn), mysql_errno($this->conn)));
        }
        return true;
    }

    function execute($sql, $inputarr = null, $throw = true)
    {
        if (is_array($inputarr)) {
            $sql = $this->bind($sql, $inputarr);
        }
        if ($this->enableLog) {
            $this->log[] = $sql;
            log_message("sql: {$sql}", 'debug');
        }

        $this->querycount++;
        $result = mysql_query($sql, $this->conn);
        if ($result !== false) {
            $this->lasterr = null;
            $this->lasterrcode = null;
            return $result;
        }
        $this->lasterr = mysql_error($this->conn);
        $this->lasterrcode = mysql_errno($this->conn);

        if ($throw) {
            FLEA::loadClass('FLEA_Db_Exception_SqlQuery');
            __THROW(new FLEA_Db_Exception_SqlQuery($sql, $this->lasterr, $this->lasterrcode));
        }
        return false;
    }

    function qstr($value)
    {
        if (is_int($value) || is_float($value)) { return $value; }
        if (is_bool($value)) { return $value ? $this->TRUE_VALUE : $this->FALSE_VALUE; }
        if (is_null($value)) { return $this->NULL_VALUE; }
        return "'" . mysql_real_escape_string($value, $this->conn) . "'";
    }

    function qtable($tableName, $schema = null)
    {
        return $schema != '' ? "`{$schema}`.`{$tableName}`" : "`{$tableName}`";
    }

    function qfield($fieldName, $tableName = null, $schema = null)
    {
        $fieldName = ($fieldName == '*') ? '*' : "`{$fieldName}`";
        return $tableName != '' ? $this->qtable($tableName, $schema) . '.' . $fieldName : $fieldName;
    }

    function _insertId()
    {
        return mysql_insert_id($this->conn);
    }

    function _affectedRows()
    {
        return mysql_affected_rows($this->conn);
    }

    function fetchRow($res)
    {
        return mysql_fetch_row($res);
    }

    function fetchAssoc($res)
    {
        return mysql_fetch_assoc($res);
    }

    function freeRes($res)
    {
        return mysql_free_result($res);
    }

    function selectLimit($sql, $length = null, $offset = null)
    {
        if (!is_null($offset)) {
            $sql .= " LIMIT " . (int)$offset;
            if (!is_null($length)) {
                $sql .= ', ' . (int)$length;
            } else {
                $sql .= ', 4294967294';
            }
        } elseif (!is_null($length)) {
            $sql .= " LIMIT " . (int)$length;
        }
        return $this->execute($sql);
    }

    function metaColumns($table)
    {
        /**
         *  C CHAR 或 VARCHAR 类型字段
         *  X TEXT 或 CLOB 类型字段
         *  B 二进制数据（BLOB）
         *  N 数值或者浮点数
         *  D 日期
         *  T TimeStamp
         *  L 逻辑布尔值
         *  I 整数
         *  R 自动增量或计数器
         */
        static $typeMap = array(
            'BIT'           => 'I',
            'TINYINT'       => 'I',
            'BOOL'          => 'L',
            'BOOLEAN'       => 'L',
            'SMALLINT'      => 'I',
            'MEDIUMINT'     => 'I',
            'INT'           => 'I',
            'INTEGER'       => 'I',
            'BIGINT'        => 'I',
            'FLOAT'         => 'N',
            'DOUBLE'        => 'N',
            'DOUBLEPRECISION' => 'N',
            'FLOAT'         => 'N',
            'DECIMAL'       => 'N',
            'DEC'           => 'N',

            'DATE'          => 'D',
            'DATETIME'      => 'T',
            'TIMESTAMP'     => 'T',
            'TIME'          => 'T',
            'YEAR'          => 'I',

            'CHAR'          => 'C',
            'NCHAR'         => 'C',
            'VARCHAR'       => 'C',
            'NVARCHAR'      => 'C',
            'BINARY'        => 'B',
            'VARBINARY'     => 'B',
            'TINYBLOB'      => 'X',
            'TINYTEXT'      => 'X',
            'BLOB'          => 'X',
            'TEXT'          => 'X',
            'MEDIUMBLOB'    => 'X',
            'MEDIUMTEXT'    => 'X',
            'LONGBLOB'      => 'X',
            'LONGTEXT'      => 'X',
            'ENUM'          => 'C',
            'SET'           => 'C',
        );

        $rs = $this->execute(sprintf($this->META_COLUMNS_SQL, $table));
        if (!$rs) { return false; }
        $retarr = array();
        while (($row = mysql_fetch_assoc($rs))) {
            $field = array();
            $field['name'] = $row['Field'];
            $type = $row['Type'];

            $field['scale'] = null;
            $queryArray = false;
            if (preg_match('/^(.+)\((\d+),(\d+)/', $type, $queryArray)) {
                $field['type'] = $queryArray[1];
                $field['maxLength'] = is_numeric($queryArray[2]) ? $queryArray[2] : -1;
                $field['scale'] = is_numeric($queryArray[3]) ? $queryArray[3] : -1;
            } elseif (preg_match('/^(.+)\((\d+)/', $type, $queryArray)) {
                $field['type'] = $queryArray[1];
                $field['maxLength'] = is_numeric($queryArray[2]) ? $queryArray[2] : -1;
            } elseif (preg_match('/^(enum)\((.*)\)$/i', $type, $queryArray)) {
                $field['type'] = $queryArray[1];
                $arr = explode(",",$queryArray[2]);
                $field['enums'] = $arr;
                $zlen = max(array_map("strlen",$arr)) - 2; // PHP >= 4.0.6
                $field['maxLength'] = ($zlen > 0) ? $zlen : 1;
            } else {
                $field['type'] = $type;
                $field['maxLength'] = -1;
            }
            $field['simpleType'] = $typeMap[strtoupper($field['type'])];
            // if ($field['simpleType'] == 'C' && $field['maxLength'] > 250) {
                // $field['simpleType'] = 'X';
            // }
            $field['notNull'] = ($row['Null'] != 'YES');
            $field['primaryKey'] = ($row['Key'] == 'PRI');
            $field['autoIncrement'] = (strpos($row['Extra'], 'auto_increment') !== false);
            if ($field['autoIncrement']) { $field['simpleType'] = 'R'; }
            $field['binary'] = (strpos($type,'blob') !== false);
            $field['unsigned'] = (strpos($type,'unsigned') !== false);

            if ($field['type'] == 'tinyint' && $field['maxLength'] == 1) {
                $field['simpleType'] = 'L';
            }

            if (!$field['binary']) {
                $d = $row['Default'];
                if ($d != '' && $d != 'NULL') {
                    $field['hasDefault'] = true;
                    $field['defaultValue'] = $this->setValueByType($d, $field['simpleType']);
                } else {
                    $field['hasDefault'] = false;
                }
            }

            $field['description'] = isset($row['Comment']) ? $row['Comment'] : '';

            $retarr[strtoupper($field['name'])] = $field;
        }
        mysql_free_result($rs);
        return $retarr;
    }

    function metaTables($pattern = null, $schema = null)
    {
        $sql = 'SHOW TABLES';
		if (!empty($schema)) {
		    $sql .= " FROM {$schema}";
		}
		if (!empty($pattern)) {
		    $sql .= ' LIKE ' . $this->qstr($schema);
		}
		$res = $this->execute($sql, null, false);
		$tables = array();
		while (($row = $this->fetchRow($res))) {
		   $tables[] = reset($row);
		}
		$this->freeRes($res);
		return $tables;
    }

}

/**
 * 与 FLEA_Db_Driver_Mysql 的唯一区别在于 FLEA_Db_Driver_Mysqlt 支持事务功能
 *
 * 要求表的存储引擎为 InnoDB 或者 BDB。
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.1
 */
class FLEA_Db_Driver_Mysqlt extends FLEA_Db_Driver_Mysql
{
    var $HAS_TRANSACTION = true;

    function connect($dsn = false)
    {
        parent::connect($dsn);
        if ($this->_mysqlVersion >= '5.0') {
            $this->HAS_SAVEPOINT = true;
        }
    }

    function _startTrans()
    {
        $this->execute('START TRANSACTION');
    }

    function _completeTrans($commitOnNoErrors = true)
    {
        if ($this->_hasFailedQuery == false && $commitOnNoErrors) {
            $this->execute('COMMIT');
        } else {
            $this->execute('ROLLBACK');
        }
    }
}
