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
 * 定义 FLEA_Db_Driver_Mssql 驱动
 *
 * 非常感谢 IceLoad 为 FleaPHP 贡献的 MSSQL 驱动，让 FleaPHP 可以方便的操作 Microsoft SQL Server 数据库。
 * 支持的 MSSQL 数据库包括 MSDE2000、MSSQL2000/2005（含 Express 版）。
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author IceLoad
 * @package Core
 * @version $Id: Mssql.php 1060 2008-05-04 05:02:59Z qeeyuan $
 */

// {{{ includes
FLEA::loadClass('FLEA_Db_Driver_Abstract');
// }}}

// {{{ init
if (!class_exists('FLEA_Db_Driver_Mssql')) {
    // 避免 MSSQL 转换日期格式，以便返回 YYYY-MM-DD hh:mm:ss 格式的日期
    ini_set('mssql.datetimeconvert', 0);
}
// }}}

/**
 * 用于 mssql 扩展的数据库驱动程序
 *
 * @package Core
 * @author IceLoad
 * @version 1.1
 */
class FLEA_Db_Driver_Mssql extends FLEA_Db_Driver_Abstract
{
    var $META_COLUMNS_SQL = "SELECT a.COLUMN_NAME AS Field, (CASE WHEN CHARACTER_MAXIMUM_LENGTH IS NULL THEN DATA_TYPE + '(' + CONVERT(VARCHAR,NUMERIC_PRECISION) + ')' ELSE DATA_TYPE + '(' + CONVERT(VARCHAR,CHARACTER_MAXIMUM_LENGTH) + ')' END) AS DataType, IS_NULLABLE AS Nullable, CONSTRAINT_TYPE AS PrimaryKey, COLUMN_DEFAULT AS DefaultValue, (CASE WHEN COLUMNPROPERTY(OBJECT_ID('%s'),a.COLUMN_NAME,'IsIdentity')=1 THEN 'auto_increment' ELSE '' END) AS Extra FROM INFORMATION_SCHEMA.COLUMNS AS a LEFT JOIN (INFORMATION_SCHEMA.KEY_COLUMN_USAGE AS b INNER JOIN INFORMATION_SCHEMA.TABLE_CONSTRAINTS AS c ON b.CONSTRAINT_NAME=c.CONSTRAINT_NAME  AND b.TABLE_NAME=c.TABLE_NAME) ON a.COLUMN_NAME=b.COLUMN_NAME AND a.TABLE_NAME=b.TABLE_NAME WHERE a.TABLE_NAME='%s'";
    var $PARAM_STYLE = DBO_PARAM_AT_NAMED;
    var $HAS_INSERT_ID  = true;
    var $HAS_AFFECTED_ROWS = true;

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
		$this->conn = mssql_connect($host, $dsn['login'], $dsn['password']);

        if (!$this->conn) {
            FLEA::loadClass('FLEA_Db_Exception_SqlQuery');
            return __THROW(new FLEA_Db_Exception_SqlQuery("mssql_connect('{$host}', '{$dsn['login']}') failed!", $this->_error(), $this->_errno()));
        }

        if (!mssql_select_db($dsn['database'], $this->conn)) {
            FLEA::loadClass('FLEA_Db_Exception_SqlQuery');
            return __THROW(new FLEA_Db_Exception_SqlQuery("SELECT DATABASE: '{$dsn['database']}' FAILED!", $this->_error($this->conn), $this->_errno($this->conn)));
        }

        return true;
    }

    /**
     * 关闭数据库连接
     */
    function close()
    {
        if ($this->conn) { mssql_close($this->conn); }
        parent::close();
    }

    function selectDb($database)
    {
        if (!mssql_select_db($dsn['database'], $this->conn)) {
            FLEA::loadClass('FLEA_Db_Exception_SqlQuery');
            return __THROW(new FLEA_Db_Exception_SqlQuery("SELECT DATABASE: '{$database}' FAILED!", $this->_error($this->conn), $this->_errno($this->conn)));
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
        $result = mssql_query($sql, $this->conn);
        if ($result !== false) {
            $this->lasterr = null;
            $this->lasterrcode = null;
            return $result;
        }
        $this->lasterr = $this->_error($this->conn);
        $this->lasterrcode = $this->_errno($this->conn);

        if ($throw) {
            FLEA::loadClass('FLEA_Db_Exception_SqlQuery');
            __THROW(new FLEA_Db_Exception_SqlQuery($sql, $this->lasterr, $this->lasterrcode));
        }
        return false;
    }

    function qstr($value)
    {
        if (is_int($value)) { return $value; }
        if (is_bool($value)) { return $value ? $this->TRUE_VALUE : $this->FALSE_VALUE; }
        if (is_null($value)) { return $this->NULL_VALUE; }
        return str_replace("'", "''", $value);
    }

    function qtable($tableName, $schema = null)
    {
        return $schema != '' ? "[{$schema}].[{$tableName}]" : "[{$tableName}]";
    }

    function qfield($fieldName, $tableName = null, $schema = null)
    {
        $fieldName = ($fieldName == '*') ? '*' : "[{$fieldName}]";
        return $tableName != '' ? $this->qtable($tableName, $schema) . '.' . $fieldName : $fieldName;
    }

    function _insertId()
    {
        return $this->getOne('SELECT SCOPE_IDENTITY()');
    }

    function _affectedRows()
    {
        return $this->getOne('SELECT @@rowcount');
    }

    function fetchRow($res)
    {
        return mssql_fetch_row($res);
    }

    function fetchAssoc($res)
    {
        return mssql_fetch_assoc($res);
    }

    function freeRes($res)
    {
        return mssql_free_result($res);
    }

    function selectLimit($sql, $length = null, $offset = null)
    {

    }

    function metaColumns($table)
    {
        /**
         *  C 长度小于等于 250 的字符串
         *  X 长度大于 250 的字符串
         *  B 二进制数据
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
			'REAL'          => 'N',
			'NUMERIC'       => 'N',
            'DECIMAL'       => 'N',
            'DEC'           => 'N',
			'MONEY'         => 'N',
			'SMALLMONEY'    => 'N',


            'DATE'          => 'D',
            'DATETIME'      => 'T',
			'SMALLDATETIME' => 'T',
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
			'NTEXT'         => 'X',
            'MEDIUMBLOB'    => 'X',
            'MEDIUMTEXT'    => 'X',
            'LONGBLOB'      => 'X',
            'LONGTEXT'      => 'X',
            'ENUM'          => 'C',
            'SET'           => 'C',
        );

        //$table = $this->qtable($table);echo sprintf($this->META_COLUMNS_SQL, $table, $table);exit;
        $rs = $this->execute(sprintf($this->META_COLUMNS_SQL, $table, $table));
        if (!$rs) { return false; }
        $retarr = array();
        while (($row = mssql_fetch_row($rs))) {
            $field = array();
            $field['name'] = $row[0];
            $type = $row[1];

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
            if ($field['simpleType'] == 'C' && $field['maxLength'] > 255) {
                $field['simpleType'] = 'X';
            }
            $field['notNull'] = ($row[2] != 'YES');
			if (is_null($row[3])) { $row[3] = ''; }
            $field['primaryKey'] = ($row[3] == 'PRIMARY KEY');
            $field['autoIncrement'] = (strpos($row[5], 'auto_increment') !== false);
            if ($field['autoIncrement']) { $field['simpleType'] = 'R'; }
            $field['binary'] = (strpos($type,'blob') !== false);
            $field['unsigned'] = (strpos($type,'unsigned') !== false);

            if (!$field['binary']) {
                $d = is_null($row[4]) ? '' : $row[4];
				if ($d != '') {
					if (substr($d, 0, 1) == '(') {
						$d = substr($d, 1, strlen($d) - 2);
						if (substr($d, 0, 1) == "'") { $d = substr($d, 1, strlen($d) - 2); }
					}
				}
                if ($d != '') {
                    $field['hasDefault'] = true;
                    $field['defaultValue'] = $d;
                } else {
                    $field['hasDefault'] = false;
                }
            }
            $retarr[strtoupper($field['name'])] = $field;
        }
        mssql_free_result($rs);
        return $retarr;
    }

    /**
     * 为数据表产生下一个序列值
     *
     * @param string $seqName
     * @param string $startValue
     *
     * @return int
     */
    function nextId($seqName = 'sdboseq', $startValue = 1)
    {
		$this->execute('BEGIN TRANSACTION ' . $seqName);
		$ok = $this->execute(sprintf($this->NEXT_ID_SQL, $seqName), null, false);
		if ($ok === false) {
			$this->execute(sprintf($this->CREATE_SEQ_SQL, $seqName));
			$ok = $this->execute(sprintf($this->INIT_SEQ_SQL, $seqName, $startValue));
			if (!$ok) {
				$this->execute('ROLLBACK TRANSACTION ' . $seqName);
				return false;
			}
			$this->execute('COMMIT TRANSACTION ' . $seqName);
			return $startValue;
		}
		$num = $this->query_scalar_function('SELECT id FROM ' . $seqName);
		$this->execute('COMMIT TRANSACTION ' . $seqName);
		return $num;
    }

    /**
     * 创建一个新的序列，成功返回 true，失败返回 false
     *
     * @param string $seqName
     * @param int $startValue
     *
     * @return boolean
     */
    function createSeq($seqName = 'sdboseq', $startValue = 1)
    {
		$this->execute('BEGIN TRANSACTION ' . $seqName);
		$this->execute(sprintf($this->CREATE_SEQ_SQL, $seqName));
		$ok = $this->execute(sprintf($this->INIT_SEQ_SQL, $seqName, $startValue - 1));
		if (!$ok) {
			$this->execute('ROLLBACK TRANSACTION ' . $seqName);
			return false;
		}
		$this->execute('COMMIT TRANSACTION ' . $seqName);
		return true;
    }

    /**
     * 删除一个序列
     *
     * 具体的实现与数据库系统有关。
     *
     * @param string $seqName
     */
    function dropSeq($seqName = 'sdboseq')
    {
        return $this->execute(sprintf($this->DROP_SEQ_SQL, $seqName));
    }

	function numRows($res)
	{
		return mssql_num_rows($res);
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
        return mssql_fetch_row($res);
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
        return mssql_fetch_assoc($res);
    }

    /**
     * 释放查询句柄
     *
     * @param resource $res
     *
     * @return boolean
     */
    function freeRes($res)
    {
        return mssql_free_result($res);
    }

    /**
     * 进行限定记录集的查询
     *
     * @param string $sql
     * @param int $length
     * @param int $offset
     *
     * @return resource
     */
    function selectLimit($sql, $length = null, $offset = null)
    {
		if(is_numeric($length)){
			$intPageSize = intval($length);
			if($intPageSize < 0){$intPageSize = 0;}
		}else{
			$intPageSize = 0;
		}
		if(is_numeric($offset)){
			$intStartPosition = intval($offset);
			if($intStartPosition < 0){$intStartPosition = 0;}
		}else{
			$intStartPosition = 0;
		}
		$strSQL = $sql;
		$strPattern = '/^\s*(SELECT\s+(ALL|DISTINCT)?(\s+TOP\s+\d+)?(.+))(\s+FROM\s+.+)';
		if(strripos($strSQL, 'WHERE')){
			$strPattern .= '(\s+WHERE\s+.+)';
		}
		if(strripos($strSQL, 'GROUP BY')){
			$strPattern .= '(\s+GROUP BY\s+.+)';
		}
		if(strripos($strSQL, 'HAVING')){
			$strPattern .= '(\s+HAVING\s+.+)';
		}
		if(strripos($strSQL, 'ORDER BY')){
			$strPattern .= '(\s+ORDER BY\s+.+)';
		}
		$strPattern .= '$/i';
		$arrMatches = array();
		if(preg_match($strPattern, $strSQL, $arrMatches)){
			$j = count($arrMatches);
			for($i = 0; $i < $j; $i ++){
				$arrMatches[$i] = trim($arrMatches[$i]);
			}
			if(empty($arrMatches[3]) && $j > 5){
				$strLimitSql = 'SELECT ' . $arrMatches[2] . ' TOP ' . $intPageSize . ' ' . $arrMatches[4];
				$strLimitSql .= ' ' . $arrMatches[5];
				$strAlias = '';
				if(strpos($arrMatches[5], ',')){
					$strAlias = substr($arrMatches[5], 0, strpos($arrMatches[5], ','));
				}elseif(stristr($arrMatches[5], " JOIN ")){
					$strAlias = stristr($arrMatches[5], " JOIN ");
					$strAlias = substr($arrMatches[5], 0, strpos($arrMatches[5], $strAlias));
				}
				if(! empty($strAlias)){
					$strAlias = trim(substr($strAlias, 4));
					$arrAlias = split(' ', $strAlias);
					$strAlias = $arrAlias[0];
					if(strtoupper($arrAlias[1]) == 'AS'){
						$strAlias = $arrAlias[2];
					}elseif(! in_array(strtoupper($arrAlias[1]), array('INNER','LEFT','JOIN','RIGHT','FULL'))){
						$strAlias = $arrAlias[1];
					}
					//$strAlias = trim(substr($strAlias, strrpos($strAlias, ' ')));
					if(! empty($strAlias))$strAlias .= '.';
				}
				if($j > 6){
					if(strtoupper(substr($arrMatches[6], 0, 5)) == 'WHERE'){
						$strLimitSql .= ' WHERE (' . substr($arrMatches[6], 5) . ') AND ' . $strAlias . 'IDENTITYCOL NOT IN (';
						$strLimitSql .= 'SELECT ' . $arrMatches[2] . ' TOP ' . $intStartPosition . ' ' . $strAlias . 'IDENTITYCOL ' . $arrMatches[5];
						for($i = 6; $i < $j; $i ++){
							$strLimitSql .= ' ' . $arrMatches[$i];
						}
						$strLimitSql .= ')';
						for($i = 7; $i < $j; $i ++){
							$strLimitSql .= ' ' . $arrMatches[$i];
						}
					}else{
						$strLimitSql .= ' WHERE ' . $strAlias . 'IDENTITYCOL NOT IN (';
						$strLimitSql .= 'SELECT ' . $arrMatches[2] . ' TOP ' . $intStartPosition . ' ' . $strAlias . 'IDENTITYCOL ' . $arrMatches[5];
						for($i = 6; $i < $j; $i ++){
							$strLimitSql .= ' ' . $arrMatches[$i];
						}
						$strLimitSql .= ')';
						for($i = 6; $i < $j; $i ++){
							$strLimitSql .= ' ' . $arrMatches[$i];
						}
					}
				}else{
					$strLimitSql .= ' WHERE ' . $strAlias . 'IDENTITYCOL NOT IN (';
					$strLimitSql .= 'SELECT ' . $arrMatches[2] . ' TOP ' . $intStartPosition . ' ' . $strAlias . 'IDENTITYCOL ' . $arrMatches[5];
					$strLimitSql .= ')';
				}
				return $this->execute($strLimitSql);
			}
			return false;
		}
		return false;
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
        while ($row = mssql_fetch_assoc($res)) {
            $data[] = $row;
        }
        mssql_free_result($res);
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
    function & getAllGroupBy($sql, $groupBy)
    {
        if (is_resource($sql)) {
            $res = $sql;
        } else {
            $res = $this->execute($sql);
        }
        $data = array();

        $row = mssql_fetch_assoc($res);
        if ($row != false) {
            if ($groupBy === true) {
                $groupBy = key($row);
            }
            do {
                $rkv = $row[$groupBy];
                unset($row[$groupBy]);
                $data[$rkv][] = $row;
            } while ($row = mssql_fetch_assoc($res));
        }

        mssql_free_result($res);

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
        while ($row = mssql_fetch_assoc($res)) {
            $fieldValue = $row[$field];
            unset($row[$field]);
            $data[$offset] = $row;
            $fieldValues[$offset] = $fieldValue;
            $reference[$fieldValue] =& $data[$offset];
            $offset++;
        }
        mssql_free_result($res);
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
            if ($limit !== null) {
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
            while ($row = mssql_fetch_assoc($res)) {
                $rkv = $row[$refKeyName];
                unset($row[$refKeyName]);
                $assocRowset[$rkv][$mappingName] = $row;
            }
        } else {
            // 一对多组装数据
            while ($row = mssql_fetch_assoc($res)) {
                $rkv = $row[$refKeyName];
                unset($row[$refKeyName]);
                $assocRowset[$rkv][$mappingName][] = $row;
            }
        }

        mssql_free_result($res);
    }

    /**
     * 执行查询，返回第一条记录的第一个字段
     *
     * @param string|resource $sql
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
        $row = mssql_fetch_row($res);
        mssql_free_result($res);
        return isset($row[0]) ? $row[0] : null;
    }

    /**
     * 执行查询，返回第一条记录
     *
     * @param string|resource $sql
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
        $row = mssql_fetch_assoc($res);
        mssql_free_result($res);
        return $row;
    }

    /**
     * 执行查询，返回结果集的指定列
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
        while ($row = mssql_fetch_row($res)) {
            $data[] = $row[$col];
        }
        mssql_free_result($res);
        return $data;
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
        return false;
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
        return true;
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
            } else if ($v === null) {
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
     * 返回上一个 MsSQL 函数的错误号码
     *
     * @param resource link_identifier $conn
     *
     * @return int
     */
	function _errno()
	{
		$errorSQL = "SELECT @@ERROR";
		if (empty($conn)) {
			$num = $this->query_scalar_function($errorSQL);
		} else {
			$num = $this->query_scalar_function($errorSQL, $conn);
		}
		return $num;
	}

    /**
     * 返回上一个 MsSQL 函数的错误文本
     *
     * @param resource link_identifier $conn
     *
     * @return string
     */
	function _error()
	{
		return mssql_get_last_message();
	}

    /**
     * 取得上一步 INSERT 操作产生的 ID
     *
     * @param resource link_identifier $conn
     *
     * @return int
     */
	function mssql_insert_id(&$conn = false)
	{
		$identitySQL = 'SELECT @@IDENTITY'; // 'SELECT SCOPE_IDENTITY'; 'SELECT IDENT_CURRENT(table_name)' # for mssql 2000
		if (empty($conn)) {
			$id = $this->query_scalar_function($identitySQL);
		} else {
			$id = $this->query_scalar_function($identitySQL, $conn);
		}
		return $id;
	}

	function mssql_real_escape_string($value, &$conn = false)
	{
		if (get_magic_quotes_gpc()) {
			$value = stripslashes($value);
		}
		$value = str_replace("'", "''", $value);
		return $value;
	}

	function query_scalar_function($sql, &$conn = false)
	{
		if (empty($conn)) {
			$rs = @mssql_query($sql, $this->conn);
		} else {
			$rs = @mssql_query($sql, $conn);
		}
		if (!$rs) return false;
		$arr = mssql_fetch_array($rs);
		mssql_free_result($rs);
		if (is_array($arr)) {
			return $arr[0];
		} else {
			return -1;
		}
	}
}

/**
 * 与 FLEA_Db_Driver_Mssql 的唯一区别在于 FLEA_Db_Driver_Mssqlt 支持事务功能
 *
 * 要求表的存储引擎为 InnoDB 或者 BDB。
 *
 * @package Core
 * @author 廖宇雷 dualface@gmail.com
 * @version 1.1
 */
class FLEA_Db_Driver_Mssqlt extends FLEA_Db_Driver_Mssql
{
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
     * 启动事务
     */
    function startTrans()
    {
        $this->_transCount += 1;
        if ($this->_transCount == 1) {
            $this->execute('BEGIN TRAN');
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
        if ($this->_transCount < 1) { return false; }
        if ($this->_transCount > 1) {
            $this->_transCount -= 1;
            return true;
        }
        $this->_transCount = 0;

        if ($this->_transCommit && $commitOnNoErrors) {
            $ret = $this->execute('COMMIT TRAN');
            return $ret;
        } else {
            $this->execute('ROLLBACK TRAN');
            return false;
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
}
