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
 * 定义 FLEA_View_Lite 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Lite.php 1005 2007-11-03 07:43:55Z qeeyuan $
 */

// {{{ includes

do {
    if (PHP5) {
        if (class_exists('Template_Lite', false)) { break; }
    } else {
        if (class_exists('Template_Lite')) { break; }
    }

    $viewConfig = FLEA::getAppInf('viewConfig');
    if (!isset($viewConfig['liteDir'])) {
        FLEA::loadClass('FLEA_View_Exception_NotConfigurationLite');
        return __THROW(new FLEA_View_Exception_NotConfigurationLite());
    }

    $filename = $viewConfig['liteDir'] . '/class.template.php';
    if (!file_exists($filename)) {
        FLEA::loadClass('FLEA_View_Exception_InitLiteFailed');
        return __THROW(new FLEA_View_Exception_InitLiteFailed($filename));
    }

    require($filename);
} while (false);

// }}}

/**
 * FLEA_View_Lite 提供了对 TemplateLite 模板引擎的支持
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_View_Lite extends Template_Lite
{
    /**
     * 构造函数
     *
     * @return FLEA_View_Lite
     */
    function FLEA_View_Lite() {
        parent::Template_Lite();

        $viewConfig = FLEA::getAppInf('viewConfig');
        if (is_array($viewConfig)) {
            foreach ($viewConfig as $key => $value) {
                if (isset($this->{$key})) {
                    $this->{$key} = $value;
                }
            }
        }

        FLEA::loadClass('FLEA_View_SmartyHelper');
        new FLEA_View_SmartyHelper($this);
    }
}
