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
 * 定义 FLEA_Exception_NotNotExistsKeyName 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: NotExistsKeyName.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Exception_NotExistsKeyName 异常指示需要的键名不存在
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Exception_NotExistsKeyName extends FLEA_Exception
{
    var $keyname;

    /**
     * 构造函数
     *
     * @param string $keyname
     *
     * @return FLEA_Exception_NotExistsKeyName
     */
    function FLEA_Exception_NotExistsKeyName($keyname)
    {
        $this->keyname = $keyname;
        parent::FLEA_Exception(sprintf(_ET(0x0102009), $keyname));
    }
}
