<?php
/////////////////////////////////////////////////////////////////////////////
// FleaPHP Framework
//
// 该文件由“夜猫子”共享，特此感谢！
/////////////////////////////////////////////////////////////////////////////

/**
 * 定义 FLEA_Db_Driver_Oracle 驱动
 *
 * 参考 AdoDB 的 MetaColumns() 及 SelectLimit() 方法。
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Oracle.php 1288 2008-08-26 00:49:38Z dualface $
 */

// {{{ includes
FLEA::loadClass('FLEA_Db_Driver_Abstract');
// }}}

/**
 * 用于 pgsql 扩展的数据库驱动程序
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Db_Driver_Oracle extends FLEA_Db_Driver_Abstract
{
    var $NEXT_ID_SQL    = "SELECT (%s.nextval) FROM DUAL";
    var $CREATE_SEQ_SQL = "CREATE SEQUENCE %s START WITH %s";
    var $DROP_SEQ_SQL   = "DROP SEQUENCE %s";
    var $META_COLUMNS_SQL = "SELECT CNAME, COLTYPE, WIDTH, SCALE, PRECISION, NULLS, DEFAULTVAL FROM COL WHERE TNAME=%s ORDER BY COLNO";
    var $PARAM_STYLE = DBO_PARAM_CL_NAMED;
    var $HAS_AFFECTED_ROWS = true;
    var $HAS_TRANSACTION = true;
    var $HAS_SAVEPOINT = true;
    var $RESULT_FIELD_NAME_LOWER = true;
    var $NLS_DATE_FORMAT = 'yyyy-mm-dd hh24:mi:ss';

    /**
     * 最后一次查询的结果
     */
    var $_lastrs = null;

    /**
     * 查询的提交模式
     */
    var $_commitMode = OCI_COMMIT_ON_SUCCESS;

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
        if (isset($dsn['charset']) && $dsn['charset'] != '') {
            $charset = $dsn['charset'];
        } else {
            $charset = FLEA::getAppInf('databaseCharset');
        }
        if (strtoupper($charset) == 'GB2312') { $charset = 'GBK'; }
        if (empty($dsn['database'])) {
            $dsn['database'] = null;
        }
        if ($charset != '') {
            $this->conn = ocilogon("{$dsn['login']}", $dsn['password'], $dsn['database'], $charset);
        } else {
            $this->conn = ocilogon($dsn['login'], $dsn['password'], $dsn['database']);
        }

        if (!$this->conn) {
            FLEA::loadClass('FLEA_Db_Exception_SqlQuery');
            $err = ocierror();
            __THROW(new FLEA_Db_Exception_SqlQuery("ocilogon('{$dsn['login']}') failed.", $err['message'], $err['code']));
            return false;
        }

        $this->execute("ALTER SESSION SET NLS_DATE_FORMAT = '{$this->NLS_DATE_FORMAT}'");

        return true;
    }

    function selectDb($database)
    {
        $this->execute("ALTER SESSION CURRENT_SCHEMA = {$database}");
    }

    function close()
    {
        if ($this->conn) { ocilogoff($this->conn); }
        parent::close();
    }

    function execute($sql, $inputarr = null, $throw = true)
    {
        if ($this->enableLog) {
            $this->log[] = $sql;
            log_message("sql: {$sql}", 'debug');
        }

        $this->querycount++;
        $stmt = ociparse($this->conn, $sql);

        if (is_array($inputarr)) {
            foreach (array_keys($inputarr) as $k) {
                ocibindbyname($stmt, $k, $inputarr[$k], -1);
            }
        }

        if ($stmt && ociexecute($stmt, $this->_commitMode)) {
            $this->_lastrs = $stmt;
            $this->lasterr = null;
            $this->lasterrcode = null;
            return $stmt;
        }

        $err = ocierror($stmt);
        $this->lasterr = $err['message'];
        $this->lasterrcode = $err['code'];

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
        $value = str_replace("'", "''", $value);
        return  "'" . addcslashes($value, "\000\n\r\\\032") . "'";
    }

    function qtable($tableName, $schema = null)
    {
        return $schema != '' ? "{$schema}.{$tableName}" : $tableName;
    }

    function qfield($fieldName, $tableName = null, $schema = null)
    {
        return $tableName != '' ? $this->qtable($tableName, $schema) . '.' . $fieldName : $fieldName;
    }

    function _affectedRows()
    {
        return is_resource($this->_lastrs) ? ocirowcount($this->_lastrs) : false;
    }

    function createSeq($seqName = 'sdbo_seq', $startValue = 1)
    {
        return $this->execute(sprintf($this->CREATE_SEQ_SQL, $seqName, $startValue));
    }

    function nextId($seqName = 'sdbo_seq', $startValue = 1)
    {
        $getNextIdSql = sprintf($this->NEXT_ID_SQL, $seqName);
        $result = $this->execute($getNextIdSql, null, false);
        if (!$result)
        {
            if (!$this->createSeq($seqName, $startValue)) { return false; }
            $row = array($startValue);
        }
        else
        {
            $row = $this->fetchRow($result);
            $this->freeRes($result);
        }

        $nextId = reset($row);
        $this->_insertId = $nextId;
        return $nextId;
    }

    function & fetchRow($stmt)
    {
        $row = array();
        ocifetchinto($stmt, $row, OCI_NUM | OCI_RETURN_LOBS);
        return $row;
    }

    function & fetchAssoc($stmt)
    {
        $row = array();
        ocifetchinto($stmt, $row, OCI_ASSOC | OCI_RETURN_LOBS);
        if ($this->RESULT_FIELD_NAME_LOWER) {
            $row = array_change_key_case($row, CASE_LOWER);
        }
        return $row;
    }

    function freeRes($stmt)
    {
        ocifreestatement($stmt);
    }

    /**
     * 分页算法从 adodb 修改
     */
    function selectLimit($sql, $length = 'ALL', $offset = 0)
    {
        if (strpos($sql, '/*+') !== false) {
            $sql = str_replace('/*+ ', '/*+FIRST_ROWS ', $sql);
        } else {
            $sql = preg_replace('/^[ \t\n]*SELECT/i', 'SELECT /*+FIRST_ROWS*/', $sql);
        }

        $selectOffsetAlg1 = 100;
        $inputarr = array();
        if ($offset < $selectOffsetAlg1) {
            if ($length > 0) {
                if ($offset > 0) { $length += $offset; }
                $sql = "SELECT * FROM ({$sql}) WHERE ROWNUM <= :length";
                $inputarr['length'] = $length;
            }
            $stmt = $this->execute($sql, $inputarr);
            for ($i = 0; $i < $offset; $i++) {
                ocifetch($stmt);
            }
            return $stmt;
        } else {
             // Algorithm by Tomas V V Cox, from PEAR DB oci8.php

             // Let Oracle return the name of the columns
            $qfields = "SELECT * FROM ({$sql}) WHERE NULL = NULL";
            $stmt = ociparse($this->conn, $qfields);
            if (!$stmt) { return false; }

            if (is_array($inputarr)) {
                foreach (array_keys($inputarr) as $k) {
                    ocibindbyname($stmt, $k, $inputarr[$k], -1);
                }
            }

            if (!ociexecute($stmt, OCI_DEFAULT)) {
                ocifreestatement($stmt);
                return false;
            }

            $ncols = ocinumcols($stmt);
            for ($i = 1; $i <= $ncols; $i++) {
                $cols[] = '"' . ocicolumnname($stmt, $i) . '"';
            }

            ocifreestatement($stmt);
            $fields = implode(', ', $cols);
            $length += $offset;
            $offset += 1; // in Oracle rownum starts at 1

            $sql = "SELECT {$fields} FROM " .
                   "(SELECT rownum as adodb_rownum, {$fields} FROM " .
                   "({$sql})" .
                   ' WHERE rownum <= :adodb_nrows) WHERE adodb_rownum >= :adodb_offset';
            $inputarr['adodb_nrows'] = $length;
            $inputarr['adodb_offset'] = $offset;

            return $this->execute($sql, $inputarr);
        }
    }

    function & metaColumns($table, $normalize = true)
    {
        static $typeMap = array(
            'VARCHAR' => 'C',
            'VARCHAR2' => 'C',
            'CHAR' => 'C',
            'VARBINARY' => 'C',
            'BINARY' => 'C',
            'NCHAR' => 'C',
            'NVARCHAR' => 'C',
            'NVARCHAR2' => 'C',

            'NCLOB' => 'X',
            'LONG' => 'X',
            'LONG VARCHAR' => 'X',
            'CLOB' => 'X',

            'LONG RAW' => 'B',
            'LONG VARBINARY' => 'B',
            'BLOB' => 'B',

            'DATE' => 'D',


            'TIMESTAMP' => 'T',

            'INT' => 'I',
            'SMALLINT' => 'I',
            'INTEGER' => 'I',
        );

        $arr = explode('.', $table);
        $table = $this->qstr(strtoupper($arr[count($arr) - 1]));
        $stmt = $this->execute(sprintf($this->META_COLUMNS_SQL, $table));
        if (!$stmt) { return false; }

        $retarr = array();
        $row = array();
        while (ocifetchinto($stmt, $row, OCI_NUM)) {
            $field = array();
            if ($this->RESULT_FIELD_NAME_LOWER) {
                $field['name'] = strtolower($row[0]);
            } else {
                $field['name'] = $row[0];
            }
            $field['type'] = preg_replace('/\([0-9]+\)/', '', $row[1]);
            $field['maxLength'] = $row[2];
            $field['scale'] = isset($row[3]) ? $row[3] : null;
            if ($field['type'] == 'NUMBER') {
                if ($field['scale'] == 0) { $field['type'] = 'INT'; }
                $field['maxLength'] = isset($row[4]) ? $row[4] : null;
            }
            $field['notNull'] = (strncmp($row[5], 'NOT',3) === 0);
            $field['binary'] = (strpos($field['type'], 'BLOB') !== false);
            $field['hasDefault'] = isset($row[6]);
            $field['defaultValue'] = isset($row[6]) ? $row[6] : null;

            $t = strtoupper($field['type']);
            if (isset($typeMap[$t])) {
                $field['simpleType'] = $typeMap[$t];
            } else {
                $field['simpleType'] = 'N';
            }
            $field['autoIncrement'] = false;
            $field['primaryKey'] = false;

            if ($normalize) {
                $retarr[strtoupper($field['name'])] = $field;
            } else {
                $retarr[$field['name']] = $field;
            }
        }
        ocifreestatement($stmt);

        // 确定主键字段
        $ptab = 'USER_';
        $sql = "SELECT /*+ RULE */ distinct b.column_name FROM {$ptab}CONSTRAINTS a, {$ptab}CONS_COLUMNS b WHERE ( UPPER(b.table_name) = ({$table}))  AND (UPPER(a.table_name) = ({$table}) and a.constraint_type = 'P') AND (a.constraint_name = b.constraint_name)";
        $stmt = $this->execute($sql);
        if ($stmt) {
            $row = array();
            while (ocifetchinto($stmt, $row, OCI_NUM)) {
                $pkname = strtoupper($row[0]);
                if (isset($retarr[$pkname])) {
                    $retarr[$pkname]['primaryKey'] = true;
                    if ($retarr[$pkname]['type'] == 'INT') {
                        $retarr[$pkname]['simpleType'] = 'R';
                    }
                }
            }
            ocifreestatement($stmt);
        }

        return $retarr;
    }

    function dbTimeStamp($timestamp)
    {
        if (empty($timestamp) && $timestamp !== 0) { return 'null'; }
        return date('Y-m-d h:i:s', $timestamp);
    }

    function _startTrans()
    {
        $this->_commitMode = OCI_DEFAULT;
    }

    function _completeTrans($commitOnNoErrors = true)
    {
        if ($this->_hasFailedQuery == false && $commitOnNoErrors) {
            $this->execute('COMMIT');
        } else {
            $this->execute('ROLLBACK');
        }
        $this->_commitMode = OCI_COMMIT_ON_SUCCESS;
    }
}
