<?php

/**
 * 定义 FLEA_Db_Driver_Sqlite 驱动，用PDO实现访问sqlite3数据库
 * Editor by lonestone 2007 10 13
 * Email:wangyong.yichang@gmail.com
 * Version 0.1
 */

/**
 * 用于 sqlite 使用pdo扩展的数据库驱动程序
 */
class FLEA_Db_Driver_Sqlite
{
	/**
	 * 用于 genSeq()、dropSeq() 和 nextId() 的 SQL 查询语句
	 */
	var $NEXT_ID_SQL = "UPDATE %s SET id = LAST_INSERT_ID(id + 1)";
	var $CREATE_SEQ_SQL = "CREATE TABLE %s (id INT NOT NULL)";
	var $INIT_SEQ_SQL = "INSERT INTO %s VALUES (%s)";
	var $DROP_SEQ_SQL = "DROP TABLE %s";

	/**
	 * 用于描绘 true、false 和 null 的数据库值
	 */
	var $TRUE_VALUE = 1;
	var $FALSE_VALUE = 0;
	var $NULL_VALUE = 'NULL';

	/**
	 * 用于获取元数据的 SQL 查询语句
	 */
	var $META_COLUMNS_SQL = "SELECT sql FROM sqlite_master WHERE type='table' and name='%s'"; //sqlite 用这个表来保存数据库的SQL

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
	 * 最近一次插入操作或者 nextId() 操作返回的插入 ID
	 *
	 * @var mixed
	 */
	var $_insertId = null;

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
	 * 构造函数
	 *
	 * @param array $dsn
	 */
	function FLEA_Db_Driver_Sqlite( $dsn = false )
	{
		$tmp = ( array )$dsn;
		unset( $tmp['password'] );
		$this->dsn = $dsn;
		$this->enableLog = !defined( 'DEPLOY_MODE' ) || DEPLOY_MODE != true;
		if ( !function_exists( 'log_message' ) )
		{
			$this->enableLog = false;
		}
	}

	/**
	 * 连接数据库
	 *
	 * @param array $dsn
	 * @return boolean
	 */
	function connect( $dsn = false )
	{
		$dsn = $dsn? $dsn : $this->dsn;
		$this->conn = false;
		if ( file_exists( $dsn['db'] ) )
		{
			try{
			$this->conn = new PDO( 'sqlite2:' . $dsn['db'] ); //链接sqlite2
			} catch (PDOException $e) {
			}
			if ( !$this->conn ) $this->conn = new PDO( 'sqlite:' . $dsn['db'] ); //失败后尝试sqlite3
			
			if ( is_object( $this->conn ) )
			{
				return $this->conn;
			}
		}

		FLEA::loadClass( 'FLEA_Db_Exception_SqlQuery' );
		__THROW( new FLEA_Db_Exception_SqlQuery( "connect('{$dsn['db']}') failed! debug message:" . $ex->getMessage() ) );
		return false;
	}

	/**
	 * 关闭数据库连接
	 */
	function close()
	{
		$this->conn = null;
		$this->lasterr = null;
		$this->lasterrcode = null;
		$this->_insertId = null;
		$this->_transCount = 0;
		$this->_transCommit = true;
	}

	/**
	 * 执行一个查询，返回一个 resource 或者 boolean 值
	 *
	 * @param string $sql
	 * @param array $inputarr
	 * @param boolean $throw 指示查询出错时是否抛出异常
	 * @return resource |boolean
	 */
	function execute( $sql, $inputarr = null, $throw = true )
	{
		if ( substr( $sql, 0, 11 ) == "INSERT INTO" )
		{
			// 删除SQL中的指定的表，SQLITE不支持在插入中语句有表名在前面
			$len1 = strpos( $sql, '(' );
			$len2 = strpos( $sql, ')' );
			$len3 = strpos( $sql, 'VALUES' );
			$temp = array();
			if ( $len2 < $len3 )
			{
				$temp[] = substr( $sql, 0, $len1 );
				$temp[] = substr( $sql, $len1, $len2 - $len1 );
				$temp[] = substr( $sql, $len2 );
				$temp[1] = eregi_replace( "[a-z_0-9]+\\.", "", $temp[1] );
				$sql = implode( $temp );
			}
		}
		if ( is_array( $inputarr ) )
		{
			$sql = $this->_prepareSql( $sql, $inputarr );
		}
		if ( $this->enableLog )
		{
			$this->log[] = $sql;
			log_message( "sql:\n{$sql}", 'debug' );
		}

		$result = $this->conn->query( $sql );
		if ( $result !== false )
		{
			$this->lasterr = null;
			$this->lasterrcode = null;
			return $result;
		}
		$this->lasterrcode = $this->conn->errorCode();
		$this->lasterr = $this->conn->errorInfo();
		if ( !$throw )
		{
			return false;
		}

		FLEA::loadClass( 'FLEA_Db_Exception_SqlQuery' );
		__THROW( new FLEA_Db_Exception_SqlQuery( $sql, $this->lasterr[2], $this->lasterrcode ) );
		return false;
	}

	/**
	 * 转义字符串
	 *
	 * @param string $value
	 * @return mixed
	 */
	function qstr( $value )
	{
		if ( is_bool( $value ) )
		{
			return $value ? $this->TRUE_VALUE : $this->FALSE_VALUE;
		}
		if ( is_null( $value ) )
		{
			return $this->NULL_VALUE;
		}
		return $this->conn->quote( $value );
	}

	/**
	 * 将数据表名字转换为完全限定名
	 *
	 * @param string $tableName
	 * @return string
	 */
	function qtable( $tableName )
	{
		return $tableName; //SQLite 对转换支持不是很好，经常出错
	}

	/**
	 * 将字段名转换为完全限定名，避免因为字段名和数据库关键词相同导致的错误
	 *
	 * @param string $fieldName
	 * @param string $tableName
	 * @return string
	 */
	function qfield( $fieldName, $tableName = null )
	{
		$pos = strpos( $fieldName, '.' );
		if ( $pos !== false )
		{
			$tableName = substr( $fieldName, 0, $pos );
			$fieldName = substr( $fieldName, $pos + 1 );
		}
		if ( $tableName != '' )
		{
			if ( $fieldName != '*' )
			{
				return "{$tableName}.{$fieldName}";
			}
			else
			{
				return "{$tableName}.*";
			}
		}
		else
		{
			if ( $fieldName != '*' )
			{
				return "{$fieldName}";
			}
			else
			{
				return "*";
			}
		}
	}

	/**
	 * 一次性将多个字段名转换为完全限定名
	 *
	 * @param string $ |array $fields
	 * @param string $tableName
	 * @return string
	 */
	function qfields( $fields, $tableName = null )
	{
		if ( !is_array( $fields ) )
		{
			$fields = explode( ',', $fields );
		}
		$return = array();
		foreach ( $fields as $fieldName )
		{
			$fieldName = trim( $fieldName );
			if ( $fieldName == '' )
			{
				continue;
			}
			$pos = strpos( $fieldName, '.' );
			if ( $pos !== false )
			{
				$tableName = substr( $fieldName, 0, $pos );
				$fieldName = substr( $fieldName, $pos + 1 );
			}
			if ( $tableName != '' )
			{
				if ( $fieldName != '*' )
				{
					$return[] = "{$tableName}.{$fieldName}";
				}
				else
				{
					$return[] = "{$tableName}.*";
				}
			}
			else
			{
				if ( $fieldName != '*' )
				{
					$return[] = "{$fieldName}";
				}
				else
				{
					$return[] = '*';
				}
			}
		}
		return implode( ', ', $return );
	}

	/**
	 * 为数据表产生下一个序列值
	 *
	 * @param string $seqName
	 * @param string $startValue
	 * @return int
	 */
	function nextId( $seqName = 'sdboseq', $startValue = 1 )
	{
		$result = $this->execute( sprintf( $this->NEXT_ID_SQL, $seqName ), null, false );
		if ( $result === false )
		{
			if ( !$this->createSeq( $seqName, $startValue ) )
			{
				return false;
			}
			$this->execute( sprintf( $this->NEXT_ID_SQL, $seqName ) );
		}
		$id = $this->insertId();
		if ( $id )
		{
			return $id;
		}
		if ( $this->execute( sprintf( $this->INIT_SEQ_SQL, $seqName, $startValue ) ) )
		{
			return $startValue;
		}
		return false;
	}

	/**
	 * 创建一个新的序列，成功返回 true，失败返回 false
	 *
	 * @param string $seqName
	 * @param int $startValue
	 * @return boolean
	 */
	function createSeq( $seqName = 'sdboseq', $startValue = 1 )
	{
		if ( $this->execute( sprintf( $this->CREATE_SEQ_SQL, $seqName ) ) )
		{
			return $this->execute( sprintf( $this->INIT_SEQ_SQL, $seqName, $startValue - 1 ) );
		}
		else
		{
			return false;
		}
	}

	/**
	 * 删除一个序列

	 * 具体的实现与数据库系统有关。
	 *
	 * @param string $seqName
	 */
	function dropSeq( $seqName = 'sdboseq' )
	{
		return $this->execute( sprintf( $this->DROP_SEQ_SQL, $seqName ) );
	}

	/**
	 * 获取自增字段的最后一个值
	 *
	 * @return mixed
	 */
	function insertId()
	{
		return $this->conn->lastInsertId();
	}

	/**
	 * 返回最近一次数据库操作受到影响的记录数
	 *
	 * @return int
	 */
	function affectedRows()
	{
		return $this->conn->exec(); //这里仅对select有效
	}

	/**
	 * 从记录集中返回一行数据
	 *
	 * @param resouce $res
	 * @return array
	 */
	function fetchRow( $res )
	{
		$row = $res->fetch();
		$temp = array();
		foreach( $row as $key => $value )
		{
			$key = eregi_replace( '^[a-z0-9_]+\.', '', $key );
			$temp[$key] = $value;
		}
		return $temp;
	}

	/**
	 * 从记录集中返回一行数据，字段名作为键名
	 *
	 * @param resouce $res
	 * @return array
	 */
	function fetchAssoc( $res )
	{
		$row = $res->fetch( PDO::FETCH_ASSOC );
		$temp = array();
		foreach( $row as $key => $value )
		{
			$key = eregi_replace( '^[a-z0-9_]+\.', '', $key );
			$temp[$key] = $value;
		}
		return $temp;
	}

	/**
	 * 释放查询句柄
	 *
	 * @param resource $res
	 * @return boolean
	 */
	function freeRes( $res )
	{
		// return sqlite_free_result($res);
		return true; //sqlite 没有这样的函数
	}

	/**
	 * 进行限定记录集的查询
	 *
	 * @param string $sql
	 * @param int $length
	 * @param int $offset
	 * @return resource
	 */
	function selectLimit( $sql, $length = null, $offset = null )
	{
		if ( $offset !== null )
		{
			$sql .= "\nLIMIT " . ( int )$offset;
			if ( $length !== null )
			{
				$sql .= ', ' . ( int )$length;
			}
			else
			{
				$sql .= ', 4294967294';
			}
		}elseif ( $length !== null )
		{
			$sql .= "\nLIMIT " . ( int )$length;
		}
		return $this->execute( $sql );
	}

	/**
	 * 执行一个查询，返回查询结果记录集
	 *
	 * @param string $ |resource $sql
	 * @return array
	 */
	function & getAll( $sql )
	{
		if ( is_object( $sql ) )
		{
			$res = $sql;
		}
		else
		{
			$res = $this->execute( $sql );
		}
		$data = array();
		while ( $row = $res->fetch( PDO::FETCH_ASSOC ) )
		{
			$temp = array();
			foreach( $row as $key => $value )
			{
				$key = eregi_replace( '^[a-z0-9_]+\.', '', $key );
				$temp[$key] = $value;
			}
			$data[] = $temp;
		}
		return $data;
	}

	/**
	 * 执行一个查询，返回分组后的查询结果记录集

	 * $groupBy 参数如果为字符串或整数，表示结果集根据 $groupBy 参数指定的字段进行分组。
如果 $groupBy 参数为 true，则表示根据每行记录的第一个字段进行分组。
	 *
	 * @param string $ |resource $sql
	 * @param string $ |int|boolean $groupBy
	 * @return array
	 */
	function & getAllGroupBy( $sql, $groupBy )
	{
		if ( is_object( $sql ) )
		{
			$res = $sql;
		}
		else
		{
			$res = $this->execute( $sql );
		}
		$data = array();

		$row = $res->fetch( PDO::FETCH_ASSOC );
		if ( $row != false )
		{
			$temp = array();
			foreach( $row as $key => $value )
			{
				$key = eregi_replace( '^[a-z0-9_]+\.', '', $key );
				$temp[$key] = $value;
			}
			$row = $temp;
			if ( $groupBy === true )
			{
				$groupBy = key( $row );
			}
			do
			{
				$rkv = $row[$groupBy];
				unset( $row[$groupBy] );
				$data[$rkv][] = $row;
			}
			while ( $row = $res->fetch( PDO::FETCH_ASSOC ) );
		}
		return $data;
	}

	/**
	 * 执行一个查询，返回查询结果记录集、指定字段的值集合以及以该字段值分组后的记录集
	 *
	 * @param string $ |resource $sql
	 * @param string $field
	 * @param array $fieldValues
	 * @param array $reference
	 * @return array
	 */
	function getAllWithFieldRefs( $sql, $field, & $fieldValues, & $reference )
	{
		if ( is_object( $sql ) )
		{
			$res = $sql;
		}
		else
		{
			$res = $this->execute( $sql );
		}

		$fieldValues = array();
		$reference = array();
		$offset = 0;
		$data = array();
		while ( $row = $res->fetch( PDO::FETCH_ASSOC ) )
		{
			$temp = array();
			foreach( $row as $key => $value )
			{
				$key = eregi_replace( '^[a-z0-9_]+\.', '', $key );
				$temp[$key] = $value;
			}
			$row = $temp;
			$fieldValue = $row[$field];
			unset( $row[$field] );
			$data[$offset] = $row;
			$fieldValues[$offset] = $fieldValue;
			$reference[$fieldValue] = & $data[$offset];
			$offset++;
		}
		return $data;
	}

	/**
	 * 执行一个查询，并将数据按照指定字段分组后与 $assocRowset 记录集组装在一起
	 *
	 * @param string $ |resource $sql
	 * @param array $assocRowset
	 * @param string $mappingName
	 * @param boolean $oneToOne
	 * @param string $refKeyName
	 * @param mixed $limit
	 */
	function assemble( $sql, & $assocRowset, $mappingName, $oneToOne, $refKeyName, $limit = null )
	{
		if ( is_object( $sql ) )
		{
			$res = $sql;
		}
		else
		{
			if ( $limit !== null )
			{
				if ( is_array( $limit ) )
				{
					list( $length, $offset ) = $limit;
				}
				else
				{
					$length = $limit;
					$offset = 0;
				}
				$res = $this->selectLimit( $sql, $length, $offset );
			}
			else
			{
				$res = $this->execute( $sql );
			}
		}

		if ( $oneToOne )
		{
			// 一对一组装数据
			while ( $row = $res->fetch( PDO::FETCH_ASSOC ) )
			{
				$temp = array();
				foreach( $row as $key => $value )
				{
					$key = eregi_replace( '^[a-z0-9_]+\.', '', $key );
					$temp[$key] = $value;
				}
				$row = $temp;
				$rkv = $row[$refKeyName];
				unset( $row[$refKeyName] );
				$assocRowset[$rkv][$mappingName] = $row;
			}
		}
		else
		{
			// 一对多组装数据
			while ( $row = $res->fetch( PDO::FETCH_ASSOC ) )
			{
				$rkv = $row[$refKeyName];
				unset( $row[$refKeyName] );
				$temp = array();
				foreach( $row as $key => $value )
				{
					$key = eregi_replace( '^[a-z0-9_]+\.', '', $key );
					$temp[$key] = $value;
				}
				$assocRowset[$rkv][$mappingName][] = $temp;
			}
		}
	}

	/**
	 * 执行查询，返回第一条记录的第一个字段
	 *
	 * @param string $ |resource $sql
	 * @return mixed
	 */
	function getOne( $sql )
	{
		if ( is_object( $sql ) )
		{
			$res = $sql;
		}
		else
		{
			$res = $this->execute( $sql );
		}
		$row = $res->fetch( PDO::FETCH_NUM );
		// sqlite_free_result($res);
		return isset( $row[0] ) ? $row[0] : null;
	}

	/**
	 * 执行查询，返回第一条记录
	 *
	 * @param string $ |resource $sql
	 * @return mixed
	 */
	function & getRow( $sql )
	{
		if ( is_object( $sql ) )
		{
			$res = $sql;
		}
		else
		{
			$res = $this->execute( $sql );
		}
		$row = $res->fetch( PDO::FETCH_ASSOC );
		$temp = array();
		foreach( $row as $key => $value )
		{
			$key = eregi_replace( '^[a-z0-9_]+\.', '', $key );
			$temp[$key] = $value;
		}
		return $temp;
	}

	/**
	 * 执行查询，返回结果集的指定列
	 *
	 * @param string $ |resource $sql
	 * @param int $col 要返回的列，0 为第一列
	 * @return mixed
	 */
	function & getCol( $sql, $col = 0 )
	{
		if ( is_object( $sql ) )
		{
			$res = $sql;
		}
		else
		{
			$res = $this->execute( $sql );
		}
		$data = array();
		while ( $row = $res->fetch( PDO::FETCH_NUM ) )
		{
			$data[] = $row[$col];
		}
		return $data;
	}

	/**
	 * 返回指定表（或者视图）的元数据
	 *
	 * 部分代码参考 ADOdb 实现。

	 * 每个字段包含下列属性：
	 *
	 * name:            字段名
scale:           小数位数
	 * type:            字段类型
	 * simpleType:      简单字段类型（与数据库无关）
maxLength:       最大长度
notNull:         是否不允许保存 NULL 值
primaryKey:      是否是主键
autoIncrement:   是否是自动增量字段
binary:          是否是二进制数据
	 * unsigned:        是否是无符号数值
hasDefault:      是否有默认值
defaultValue:    默认值
	 *
	 * @param string $table
	 * @return array
	 */
	function & metaColumns( $table )
	{
		/**
		 * C 长度小于等于 250 的字符串
		 *    X 长度大于 250 的字符串
		 *    B 二进制数据
N 数值或者浮点数
		 *    D 日期
		 *    T TimeStamp
		 *    L 逻辑布尔值
I 整数
		 *    R 自动增量或计数器
		 */
		static $typeMap = array( 'BIT' => 'I',
			'TINYINT' => 'I',
			'BOOL' => 'L',
			'BOOLEAN' => 'L',
			'SMALLINT' => 'I',
			'MEDIUMINT' => 'I',
			'INT' => 'I',
			'INTEGER' => 'I',
			'BIGINT' => 'I',
			'FLOAT' => 'N',
			'DOUBLE' => 'N',
			'DOUBLEPRECISION' => 'N',
			'FLOAT' => 'N',
			'DECIMAL' => 'N',
			'DEC' => 'N',

			'DATE' => 'D',
			'DATETIME' => 'T',
			'TIMESTAMP' => 'T',
			'TIME' => 'T',
			'YEAR' => 'I',

			'CHAR' => 'C',
			'NCHAR' => 'C',
			'VARCHAR' => 'C',
			'NVARCHAR' => 'C',
			'BINARY' => 'B',
			'VARBINARY' => 'B',
			'TINYBLOB' => 'X',
			'TINYTEXT' => 'X',
			'BLOB' => 'X',
			'TEXT' => 'X',
			'MEDIUMBLOB' => 'X',
			'MEDIUMTEXT' => 'X',
			'LONGBLOB' => 'X',
			'LONGTEXT' => 'X',
			'ENUM' => 'C',
			'SET' => 'C',
			);

		$rs = $this->execute( sprintf( $this->META_COLUMNS_SQL, $table ) );
		if ( !$rs )
		{
			return false;
		}
		$retarr = array();
		$sql = $rs->fetch( PDO::FETCH_NUM );
		$sql = $sql[0];
		$firstPar = strpos( $sql, '(' );
		$endPar = strrpos( $sql, ')' )-1;
		$sql = substr( $sql, ( $firstPar + 1 ), ( $endPar - $firstPar ) );
		$sql = str_replace( "\n", '', $sql );
		$sql = str_replace( "'", '', $sql );
		$ligne = explode( ',', $sql );
		// get index key
		$sql = "select sql from sqlite_master where type='index' and tbl_name='$table'";
		$rs = $this->execute( $sql );
		$sql = $rs->fetch( PDO::FETCH_NUM );
		$sql = $sql[0];
		$firstPar = strpos( $sql, '(' );
		$endPar = strrpos( $sql, ')' )-1;
		$sql = substr( $sql, ( $firstPar + 1 ), ( $endPar - $firstPar ) );
		$sql = str_replace( "\n", '', $sql );
		$sql = str_replace( "'", '', $sql );
		$temp = explode( ',', $sql );
		$index = array();
		foreach ( $temp as $value )
		{
			$value = trim( $value );
			if ( $value )
			{
				$index[$value] = true;
			}
		}

		while ( list( $ligneNum, $cont ) = each( $ligne ) )
		{
			$row = explode( ' ', trim( $cont ) );
			$field = array();
			$field['name'] = $row[0];
			$type = $row[1];
			$field['scale'] = null;
			$queryArray = false;
			if ( preg_match( '/^(.+)\((\d+),(\d+)/', $type, $queryArray ) )
			{
				$field['type'] = $queryArray[1];
				$field['maxLength'] = is_numeric( $queryArray[2] ) ? $queryArray[2] : -1;
				$field['scale'] = is_numeric( $queryArray[3] ) ? $queryArray[3] : -1;
			}elseif ( preg_match( '/^(.+)\((\d+)/', $type, $queryArray ) )
			{
				$field['type'] = $queryArray[1];
				$field['maxLength'] = is_numeric( $queryArray[2] ) ? $queryArray[2] : -1;
			}elseif ( preg_match( '/^(enum)\((.*)\)$/i', $type, $queryArray ) )
			{
				$field['type'] = $queryArray[1];
				$arr = explode( ",", $queryArray[2] );
				$field['enums'] = $arr;
				$zlen = max( array_map( "strlen", $arr ) ) - 2; // PHP >= 4.0.6
				$field['maxLength'] = ( $zlen > 0 ) ? $zlen : 1;
			}
			else
			{
				$field['type'] = $type;
				$field['maxLength'] = -1;
			}
			$field['simpleType'] = $typeMap[strtoupper( $field['type'] )];
			if ( $field['simpleType'] == 'C' && $field['maxLength'] > 250 )
			{
				$field['simpleType'] = 'X';
			}

			$temp = eregi( 'PRIMARY[[:space:]]KEY', $cont );
			$field['primaryKey'] = $temp || $index[$row[0]];
			$field['notNull'] = $field['primaryKey'] || ( strtoupper( $row[2] ) == 'NOT' );
			$field['autoIncrement'] = $temp;
			if ( $field['autoIncrement'] )
			{
				$field['simpleType'] = 'R';
			}
			$field['binary'] = ( strpos( $type, 'blob' ) !== false );
			$field['unsigned'] = ( strpos( $type, 'unsigned' ) !== false );

			if ( !$field['binary'] )
			{
				$d = $row[4];
				if ( $d != '' && $d != 'NULL' )
				{
					$field['hasDefault'] = true;
					$field['defaultValue'] = $d;
				}
				else
				{
					$field['hasDefault'] = false;
				}
			}
			$retarr[strtoupper( $field['name'] )] = $field;
		}
		return $retarr;
	}

	/**
	 * 返回数据库可以接受的日期格式
	 *
	 * @param int $timestamp
	 */
	function dbTimeStamp( $timestamp )
	{
		return date( 'Y-m-d H:i:s', $timestamp );
	}

	/**
	 * 启动事务
	 */
	function startTrans()
	{
		$this->_transCount += 1;
		try{
		$this->conn->rollBack();
		}
		catch(PDOException $e)
		{}
		$this->conn->beginTransaction();
	}

	/**
	 * 完成事务，根据查询是否出错决定是提交事务还是回滚事务
	 *
	 * 如果 $commitOnNoErrors 参数为 true，当事务中所有查询都成功完成时，则提交事务，否则回滚事务
	 * 如果 $commitOnNoErrors 参数为 false，则强制回滚事务
	 *
	 * @param  $commitOnNoErrors 指示在没有错误时是否提交事务
	 */
	function completeTrans( $commitOnNoErrors = true )
	{
		if ( $this->_transCount < 1 )
		{
			return;
		}
		if ( $this->_transCount > 1 )
		{
			$this->_transCount -= 1;
			return;
		}
		$this->_transCount = 0;

		if ( $this->_transCommit && $commitOnNoErrors )
		{
			$this->conn->commit();
		}
		else
		{
			$this->conn->rollBack();
		}
	}

	/**
	 * 强制指示在调用 completeTrans() 时回滚事务
	 */
	function failTrans()
	{
		$this->_transCommit = true;
	}

	/**
	 * 反复事务是否失败的状态
	 */
	function hasFailedTrans()
	{
		if ( $this->_transCount > 0 )
		{
			return $this->_transCommit === false;
		}
		return false;
	}

	/**
	 * 根据 SQL 语句和提供的参数数组，生成最终的 SQL 语句
	 *
	 * @param string $sql
	 * @param array $inputarr
	 * @return string
	 */
	function _prepareSql( $sql, & $inputarr )
	{
		$sqlarr = explode( '?', $sql );
		$sql = '';
		$ix = 0;
		foreach ( $inputarr as $v )
		{
			$sql .= $sqlarr[$ix];
			$typ = gettype( $v );
			if ( $typ == 'string' )
			{
				$sql .= $this->qstr( $v );
			}
			else if ( $typ == 'double' )
			{
				$sql .= $this->qstr( str_replace( ',', '.', $v ) );
			}
			else if ( $typ == 'boolean' )
			{
				$sql .= $v ? $this->TRUE_VALUE : $this->FALSE_VALUE;
			}
			else if ( $v === null )
			{
				$sql .= 'NULL';
			}
			else
			{
				$sql .= $v;
			}
			$ix += 1;
		}
		if ( isset( $sqlarr[$ix] ) )
		{
			$sql .= $sqlarr[$ix];
		}

		return $sql;
	}
}
