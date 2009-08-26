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
 * 定义 FLEA_Db_Exception_SqlQuery 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: SqlQuery.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Db_Exception_SqlQuery 异常指示一个 SQL 语句执行错误
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Db_Exception_SqlQuery extends FLEA_Exception
{
    /**
     * 发生错误的 SQL 语句
     *
     * @var string
     */
    var $sql;

    /**
     * 构造函数
     *
     * @param string $sql
     * @param string $msg
     * @param int $code
     *
     * @return FLEA_Db_Exception_SqlQuery
     */
    function FLEA_Db_Exception_SqlQuery($sql, $msg = 0, $code = 0)
    {
        $this->sql = $sql;
        if ($msg) {
            $code = 0x06ff005;
            $msg = sprintf(_ET($code), $msg, $sql, $code);
        } else {
            $code = 0x06ff006;
            $msg = sprintf(_ET($code), $sql, $code);
        }
        parent::FLEA_Exception($msg, $code);
    }
}
