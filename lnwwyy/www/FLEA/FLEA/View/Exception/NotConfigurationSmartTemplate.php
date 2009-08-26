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
 * 定义 FLEA_View_Exception_NotConfigurationSmartTemplate 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: NotConfigurationSmartTemplate.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_View_Exception_NotConfigurationSmartTemplateSmarty 表示开发者
 * 没有为 FLEA_View_SmartTemplate 提供初始化 SmartTemplate 模版引擎需要的设置
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_View_Exception_NotConfigurationSmartTemplate extends FLEA_Exception
{
    function FLEA_View_Exception_NotConfigurationSmartTemplate()
    {
        $code = 0x0903001;
        parent::FLEA_Exception(_ET($code), $code);
    }
}
