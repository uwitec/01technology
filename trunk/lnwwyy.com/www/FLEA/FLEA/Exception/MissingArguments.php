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
 * 定义 FLEA_Exception_MissingArguments 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: MissingArguments.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Exception_MissingArguments 异常指示缺少必须的参数
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Exception_MissingArguments extends FLEA_Exception
{
    /**
     * 缺少的参数
     *
     * @var mixed
     */
    var $args;

    /**
     * 构造函数
     *
     * @param mixed $args
     *
     * @return FLEA_Exception_MissingArguments
     */
    function FLEA_Exception_MissingArguments($args)
    {
        $this->args = $args;
        if (is_array($args)) {
            $args = implode(', ', $args);
        }
        $code = 0x0102007;
        $msg = sprintf(_ET($code), $args);
        parent::FLEA_Exception($msg, $code);
    }
}
