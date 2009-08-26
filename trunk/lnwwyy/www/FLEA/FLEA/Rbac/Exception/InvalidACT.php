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
 * 定义 FLEA_Rbac_Exception_InvalidACT 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: InvalidACT.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Rbac_Exception_InvalidACT 异常指示一个无效的 ACT
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Rbac_Exception_InvalidACT extends FLEA_Exception
{
    /**
     * 无效的 ACT 内容
     *
     * @var mixed
     */
    var $act;

    /**
     * 构造函数
     *
     * @param mixed $act
     *
     * @return FLEA_Rbac_Exception_InvalidACT
     */
    function FLEA_Rbac_Exception_InvalidACT($act)
    {
        $this->act = $act;
        $code = 0x0701001;
        parent::FLEA_Exception(_ET($code), $code);
    }
}
