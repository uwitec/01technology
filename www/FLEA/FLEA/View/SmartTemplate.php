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
* 定义 FLEA_View_SmartTemplate 类
*
* @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
* @author 小龙 xlonecn@msn.com
* @package Core
* @version $Id: SmartTemplate.php 1005 2007-11-03 07:43:55Z qeeyuan $
*/

// {{{ includes

do {
    if (PHP5) {
        if (class_exists('SmartTemplate', false)) { break; }
    } else {
        if (class_exists('SmartTemplate')) { break; }
    }

    $viewConfig = FLEA::getAppInf('viewConfig');
    if (!isset($viewConfig['smartDir'])) {
        FLEA::loadClass('FLEA_View_Exception_NotConfigurationSmartTemplate');
        return __THROW(new FLEA_View_Exception_NotConfigurationSmartTemplate());
    }

    $filename = $viewConfig['smartDir'] . '/class.smarttemplate.php';
    if (!is_readable($filename)) {
        FLEA::loadClass('FLEA_View_Exception_InitSmartTemplateFailed');
        return __THROW(new FLEA_View_Exception_InitSmartTemplateFailed($filename));
    }
    require($filename);
} while (false);

// }}}

/**
* FLEA_View_SmartTemplate 提供了对 SmartTemplate 模板引擎的支持
*
* @author 小龙 xlonecn@msn.com
* @package Core
* @version 1.0
*/
class FLEA_View_SmartTemplate extends SmartTemplate
{
    /**
     * 构造函数
     *
     * @return FLEA_View_SmartTemplate
     */
    function FLEA_View_SmartTemplate()
    {
        parent::SmartTemplate();

        $viewConfig = FLEA::getAppInf('viewConfig');
        if (is_array($viewConfig)) {
            foreach ($viewConfig as $key => $value) {
                if (isset($this->{$key})) {
                    $this->{$key} = $value;
                }
            }
        }
    }

    /**
     * 输出指定模版的内容
     *
     * @param string $tpl
     */
    function display($tpl)
    {
        $this->tpl_file = $tpl;
        $this->output();
    }
}
