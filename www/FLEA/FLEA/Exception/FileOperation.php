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
 * 定义 FLEA_Exception_FileOperation 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: FileOperation.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Exception_FileOperation 异常指示文件系统操作失败
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Exception_FileOperation extends FLEA_Exception
{
    /**
     * 正在进行的文件操作
     *
     * @var string
     */
    var $operation;

    /**
     * 操作的参数
     *
     * @var array
     */
    var $args;

    /**
     * 构造函数
     *
     * @param string $opeation
     *
     * @return FLEA_Exception_FileOperation
     */
    function FLEA_Exception_FileOperation($opeation)
    {
        $this->operation = $opeation;
        $args = func_get_args();
        array_shift($args);
        $this->args = $args;
        $func = $opeation . '(' . implode(', ', $args) . ')';
        parent::FLEA_Exception(sprintf(_ET(0x0102005), $func));
    }
}
