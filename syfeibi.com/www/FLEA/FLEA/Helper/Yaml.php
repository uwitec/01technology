<?php
/**
 * Spyc -- A Simple PHP YAML Class
 * @version 0.2.3 -- 2006-02-04
 * @author Chris Wanstrath <chris@ozmm.org>
 * @link http://spyc.sourceforge.net/
 * @copyright Copyright 2005-2006 Chris Wanstrath
 * @license http://www.opensource.org/licenses/mit-license.php MIT License
 * @package Core
 */

/**
 * 载入 YAML 文件，返回分析结果
 *
 * load_yaml() 会自动使用缓存，只有当 YAML 文件被改变后，缓存才会更新。
 *
 * 关于 YAML 的详细信息,请参考 www.yaml.org 。
 *
 * 用法：
 * <code>
 * $data = load_yaml('myData.yaml');
 * </code>
 *
 * 注意：为了安全起见，不要使用 YAML 存储敏感信息，例如密码。
 * 或者将 YAML 文件的扩展名设置为 .yaml.php，并且在每一个 YAML 文件开头添加“exit()”。
 * 例如：
 * <code>
 * # <?php exit(); ?>
 *
 * invoice: 34843
 * date   : 2001-01-23
 * bill-to: &id001
 * ......
 * </code>
 *
 * 这样可以确保即便浏览器直接访问该 .yaml.php 文件，也无法看到内容。
 *
 * @param string $filename
 * @param boolean $cacheEnabled 是否缓存分析内容
 * @param array $replace
 *
 * @return array
 */
function load_yaml($filename, $cacheEnabled = true, $replace = null)
{
    static $objects = array();

    if (!file_exists($filename)) {
        FLEA::loadClass('FLEA_Exception_ExpectedFile');
        return __THROW(new FLEA_Exception_ExpectedFile($filename));
    }
    
    if ($cacheEnabled) {
        $arr = FLEA::getCache('yaml-' . $filename, filemtime($filename), false);
        if ($arr) { return $arr; }
    }

    if (!isset($objects[0])) {
        require_once FLEA_3RD_DIR . '/Spyc/spyc.php';
        $objects[0] =& new Spyc();
    }
    
    $arr = $objects[0]->load($filename, $replace);
    if ($cacheEnabled) {
        FLEA::writeCache('yaml-' . $filename, $arr);
    }
    return $arr;
}
