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
 * 定义 FLEA_Exception_ExpectedClass 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: ExpectedClass.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Exception_ExpectedClass 异常指示需要的类没有找到
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Exception_ExpectedClass extends FLEA_Exception
{
    /**
     * 类名称
     *
     * @var string
     */
    var $className;

    /**
     * 类定义文件
     *
     * @var string
     */
    var $classFile;

    /**
     * 指示文件是否存在
     *
     * @var boolean
     */
    var $fileExists;

    /**
     * 构造函数
     *
     * @param string $className
     * @param string $file
     * @param boolean $fileExists
     *
     * @return FLEA_Exception_ExpectedClass
     */
    function FLEA_Exception_ExpectedClass($className, $file = null, $fileExists = false)
    {
        $this->className = $className;
        $this->classFile = $file;
        $this->fileExists = $fileExists;
        if ($file) {
            $code = 0x0102002;
            $msg = sprintf(_ET($code), $file, $className);
        } else {
            $code = 0x0102003;
            $msg = sprintf(_ET($code), $className);
        }
        parent::FLEA_Exception($msg, $code);
    }
}
