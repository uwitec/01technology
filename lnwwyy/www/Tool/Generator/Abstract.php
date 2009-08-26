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
 * 定义 Generator_Abstract 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Scripts
 * @version $Id: Abstract.php 1000 2007-10-30 05:40:53Z qeeyuan $
 */

/**
 * Generator_Abstract 是所有生成器的基础类
 *
 * @package Scripts
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class Generator_Abstract
{
    function Generator_Abstract()
    {
        FLEA::import(FLEA::getAppInfValue('scripts', 'appDir'));
        ob_start();
        $return = include FLEA::getAppInfValue('scripts', 'appConfigFile');
        ob_clean();
        if (isset($return['dbDSN'])) {
            FLEA::setAppInf('dbDSN', $return['dbDSN']);
        }
    }

    /**
     * 继承类必须覆盖此方法
     *
     * @param array $opts
     */
    function generate($opts)
    {
    }

    /**
     * 将以“_”下划线分割的字符串转换成骆驼表示法（除第一个单词外，每个单词的第一个字母大写）
     *
     * @param string $name
     *
     * @return string
     */
    function _camelName($name)
    {
        $name = strtolower($name);
        while (($pos = strpos($name, '_')) !== false) {
            $name = substr($name, 0, $pos) . ucfirst(substr($name, $pos + 1));
        }
        return $name;
    }

    /**
     * 检查指定的类文件是否在应用程序目录中
     *
     * @param string $class
     *
     * @return string|boolean
     */
    function _existsClassFile($class)
    {
        $dir = FLEA::getAppInfValue('scripts', 'appDir');
        $filename = str_replace('_', DIRECTORY_SEPARATOR, $class) . '.php';
        $path = $dir . DIRECTORY_SEPARATOR . $filename;
        if (file_exists($path)) {
            return $path;
        } else {
            return false;
        }
    }

    /**
     * 创建指定类的定义文件
     *
     * @param string $class
     * @param string $content
     */
    function _createClassFile($class, $content)
    {
        $dir = FLEA::getAppInfValue('scripts', 'appDir');
        $filename = str_replace('_', DIRECTORY_SEPARATOR, $class) . '.php';
        $path = $dir . DIRECTORY_SEPARATOR . $filename;
        $dir = dirname($path);
        FLEA::loadHelper('file');
        mkdirs($dir);
        if (file_put_contents($path, $content)) {
            echo "Create file '{$path}' successed.\n";
            return true;
        }
        return false;
    }

    /**
     * 载入模板，返回解析结果
     *
     * @param string $tpl
     * @param array $viewdata
     *
     * @return string
     */
    function _parseTemplate($__template, $viewdata)
    {
        ob_start();
        call_user_func_array(array(__CLASS__, '_parseTemplateStatic'), array($__template, $viewdata));
        return ob_get_clean();
    }

    /**
     * 载入模板，返回解析结果（静态方法）
     *
     * @param string $tpl
     * @param array $viewdata
     *
     * @static
     */
    function _parseTemplateStatic($__template, $viewdata)
    {
        $__template = dirname(__FILE__) . '/Templates/' . $__template . '.php';
        extract($viewdata);
        return include($__template);
    }
}
