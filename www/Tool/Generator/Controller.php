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
 * 定义 Generator_Controller 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Tool
 * @version $Id: Controller.php 1000 2007-10-30 05:40:53Z qeeyuan $
 */

// {{{ includes
require_once dirname(__FILE__) . '/Abstract.php';
// }}}

/**
 * Generator_Controller 创建控制器代码文件
 *
 * @package Tool
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class Generator_Controller extends Generator_Abstract
{
    /**
     * 执行生成器
     *
     * @param array $opts
     */
    function generate($opts)
    {
        $name = array_shift($opts);
        
    }

}