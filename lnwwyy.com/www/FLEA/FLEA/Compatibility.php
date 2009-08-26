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
 * 该文件用于让基于 FleaPHP 1.0.6x 系列开发的应用程序能够在不做大量修改的情况下与
 * FleaPHP 1.0.70 及以后的版本保持兼容。
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Compatibility.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * 注册应用程序设置（已经过时，用 FLEA::loadAppInf() 代替）
 *
 * @param mixed $__config 配置数组或配置文件名
 * @deprecated
 */
function register_app_inf($__flea_internal_config = null)
{
    FLEA::loadAppInf($__flea_internal_config);
}

/**
 * 取出指定名字的设置值（已经过时，用 FLEA::getAppInf() 代替）
 *
 * @param string $option
 *
 * @return mixed
 * @deprecated
 */
function get_app_inf($option)
{
    return FLEA::getAppInf($option);
}

/**
 * 修改设置值（已经过时，用 FLEA::setAppInf() 代替）
 *
 * @param string $option
 * @param mixed $data
 * @deprecated
 */
function set_app_inf($option, $data = null)
{
    FLEA::setAppInf($option, $data);
}

/**
 * 增加文件搜索路径（已经过时，用 FLEA::import() 代替）
 *
 * @param string $dir
 * @deprecated
 */
function import($dir)
{
    FLEA::import($dir);
}

/**
 * 载入指定的文件（已经过时，用 FLEA::loadFile() 代替）
 *
 * @return boolean
 * @deprecated
 */
function load_file($filename, $loadOnce = false)
{
    return FLEA::loadFile($filename, $loadOnce);
}

/**
 * 载入指定类的定义文件（已经过时，用 FLEA::loadClass() 代替）
 *
 * @param string $filename
 *
 * @return boolean
 * @deprecated
 */
function load_class($className)
{
    return FLEA::loadClass($className);
}

/**
 * 按照 FleaPHP 中命名规则，搜索文件。成功返回文件的完整路径，失败返回 false（已经过时，用 FLEA::getFilePath() 代替）
 *
 * @param string $filename
 *
 * @return string
 * @deprecated
 */
function get_file_path($filename, $return = false)
{
    return FLEA::getFilePath($filename, $return);
}

/**
 * 返回指定对象的唯一实例（已经过时，用 FLEA::getSingleton() 代替）
 *
 * @param string $className
 *
 * @return object
 * @deprecated
 */
function & get_singleton($className)
{
    return FLEA::getSingleton($className);
}

/**
 * 将一个对象实例注册到对象实例容器（已经过时，用 FLEA::register() 代替）
 *
 * @param object $obj
 * @param string $name
 *
 * @return object
 * @deprecated
 */
function & reg(& $obj, $name = null)
{
    return FLEA::register($obj, $name);
}

/**
 * 从对象实例容其中取出指定名字的对象实例（已经过时，用 FLEA::registry() 代替）
 *
 * @param string $name
 *
 * @return object
 * @deprecated
 */
function & ref($name = null)
{
    return FLEA::registry($name);
}

/**
 * 检查指定名字的对象是否已经注册（已经过时，用 FLEA::isRegistered() 代替）
 *
 * @param string $name
 *
 * @return boolean
 * @deprecated
 */
function check_reg($name)
{
    return FLEA::isRegistered($name);
}

/**
 * 读取指定缓存的内容，如果缓存内容不存在或失效，则返回 false（已经过时，用 FLEA::getCache() 代替）
 *
 * @param string $cacheId 缓存ID，不同的缓存内容应该使用不同的ID
 * @param int $time 缓存过期时间或缓存生存周期
 * @param boolean $timeIsLifetime 指示 $time 参数的作用
 *
 * @return mixed 返回缓存的内容，缓存不存在或失效则返回 false
 * @deprecated
 */
function get_cache($cacheId, $time = 900, $timeIsLifetime = true, $cacheIdIsFilename = false)
{
    return FLEA::getCache($cacheId, $time, $timeIsLifetime, $cacheIdIsFilename);
}

/**
 * 将变量内容写入缓存（已经过时，用 FLEA::writeCache() 代替）
 *
 * @param string $cacheId
 * @param mixed $data
 *
 * @return boolean
 * @deprecated
 */
function write_cache($cacheId, $data, $cacheIdIsFilename = false)
{
    return FLEA::writeCache($cacheId, $data, $cacheIdIsFilename);
}

/**
 * 删除指定的缓存内容（已经过时，用 FLEA::purgeCache() 代替）
 *
 * @param string $cacheId
 * @deprecated
 */
function purge_cache($cacheId, $cacheIdIsFilename = false)
{
    return FLEA::purgeCache($cacheId, $cacheIdIsFilename);
}

/**
 * 输出转换 HTML 特殊字符后的文本，等同于 echo h($text)（已经过时，用 echo h() 代替）
 *
 * @param string $text
 * @deprecated
 */
function echo_h($text)
{
    echo htmlspecialchars($text);
}

/**
 * 输出转换 HTML 特殊字符、空格和换行符后的文本，等同于 echo t($text)（已经过时，用 echo t() 代替）
 *
 * @param string $text
 * @deprecated
 */
function echo_t($text)
{
    echo t($text);
}

/**
 * 返回数据库访问对象实例（已经过时，用 FLEA::getDBO() 代替）
 *
 * @param array $dsn
 *
 * @return SDBO
 * @deprecated
 */
function & get_dbo($dsn)
{
    return FLEA::getDBO($dsn);
}

/**
 * 分析 DSN 字符串，返回包含 DSN 连接信息的数组，失败返回 false（已经过时，用 FLEA::parseDSN() 代替）
 *
 * @param string $dsnString
 *
 * @return array
 * @deprecated
 */
function parse_dsn($dsnString)
{
    return FLEA::parseDSN($dsnString);
}

/**
 * 准备运行环境：载入配置文件、运行过滤器、处理 session 等（已经过时，用 FLEA::init() 代替）
 *
 * @deprecated
 */
function __FLEA_PREPARE()
{
    FLEA::init();
}

/**
 * FLEA 应用程序入口（已经过时，用 FLEA::runMVC() 代替）
 *
 * @deprecated
 */
function run()
{
    FLEA::runMVC();
}

/**
 * 初始化 Ajax，返回 FLEA_Ajax 对象实例（已经过时，用 FLEA::initAjax() 代替）
 *
 * @return FLEA_Ajax
 * @deprecated
 */
function & init_ajax()
{
    return FLEA::initAjax();
}

/**
 * 初始化 WebControls，返回 FLEA_WebControls 对象实例（已经过时，用 FLEA::initWebControls 代替）
 *
 * @return FLEA_WebControls
 * @deprecated
 */
function & init_webcontrols()
{
    return FLEA::initWebControls();
}
