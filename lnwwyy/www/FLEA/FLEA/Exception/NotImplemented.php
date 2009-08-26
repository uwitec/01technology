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
 * 定义 FLEA_Exception_NotImplemented 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: NotImplemented.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Exception_NotImplemented 异常指示某个方法没有实现
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Exception_NotImplemented extends FLEA_Exception
{
    var $className;
    var $methodName;

    /**
     * 构造函数
     *
     * @param string $method
     * @param string $class
     *
     * @return FLEA_Exception_NotImplemented
     */
    function FLEA_Exception_NotImplemented($method, $class = '')
    {
        $this->className = $class;
        $this->methodName = $method;
        if ($class) {
            $code = 0x010200a;
            parent::FLEA_Exception(sprintf(_ET($code), $class, $method));
        } else {
            $code = 0x010200b;
            parent::FLEA_Exception(sprintf(_ET($code), $method));
        }
    }
}
