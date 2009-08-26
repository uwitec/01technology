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
 * 定义 FLEA_View_Simple 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Simple.php 1304 2008-08-31 03:00:59Z dualface $
 */

/**
 * FLEA_View_Simple 实现了一个简单的、使用 PHP 自身作为模版语言，
 * 带有缓存功能的模版引擎
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_View_Simple
{
    /**
     * 模板文件所在路径
     *
     * @var string
     */
    var $path;

    /**
     * 缓存过期时间
     *
     * @access public
     * @var int
     */
    var $cacheLifetime;

    /**
     * 指示是否使用 cache
     *
     * @access public
     * @var boolean
     */
    var $enableCache;

    /**
     * 缓存文件保存位置
     *
     * @access public
     * @var string
     */
    var $cacheDir;

    /**
     * 模板变量
     *
     * @access private
     * @var array
     */
    var $vars = array();

    /**
     * 保存各个缓存内容的缓存状态
     *
     * @access private
     * @var array
     */
    var $cacheState = array();

    /**
     * 构造函数
     *
     * @param string $path 模板文件所在路径
     *
     * @return FLEA_View_Simple
     */
    function FLEA_View_Simple($path = null) {
        log_message('Construction FLEA_View_Simple', 'debug');

        $this->path = $path;
        $this->cacheLifetime = 900;
        $this->enableCache = true;
        $this->cacheDir = './cache';

		$viewConfig = (array)FLEA::getAppInf('viewConfig');
        $keys = array(
            'templateDir', 'cacheDir', 'cacheLifeTime', 'enableCache',
        );
        foreach ($keys as $key)
        {
            if (array_key_exists($key, $viewConfig))
            {
                $this->{$key} = $viewConfig[$key];
            }
        }
    }

    /**
     * 设置模板变量
     *
     * @param mixed $name 模板变量名称
     * @param mixed $value 变量内容
     */
    function assign($name, $value = null) {
        if (is_array($name) && is_null($value)) {
            $this->vars = array_merge($this->vars, $name);
        } else {
            $this->vars[$name] = $value;
        }
    }

    /**
     * 构造模板输出内容
     *
     * @param string $file 模板文件名
     * @param string $cacheId 缓存 ID，如果指定该值则会使用该内容的缓存输出
     *
     * @return string
     */
    function fetch($file, $cacheId = null) {
        if ($this->enableCache) {
            $cacheFile = $this->_getCacheFile($file, $cacheId);
            if ($this->isCached($file, $cacheId)) {
                return file_get_contents($cacheFile);
            }
        }

        // 生成输出内容并缓存
        extract($this->vars);
        ob_start();

        include($this->path . DIRECTORY_SEPARATOR . $file);
        $contents = ob_get_contents();
        ob_end_clean();

        if ($this->enableCache) {
            // 缓存输出内容，并保存缓存状态
            $this->cacheState[$cacheFile] = file_put_contents($cacheFile, $contents) > 0;
        }

        return $contents;
    }

    /**
     * 显示指定模版的内容
     *
     * @param string $file 模板文件名
     * @param string $cacheId 缓存 ID，如果指定该值则会使用该内容的缓存输出
     */
    function display($file, $cacheId = null) {
        echo $this->fetch($file, $cacheId);
    }

    /**
     * 检查内容是否已经被缓存
     *
     * @param string $file 模板文件名
     * @param string $cacheId 缓存 ID
     *
     * @return boolean
     */
    function isCached($file, $cacheId = null) {
        // 如果禁用缓存则返回 false
        if (!$this->enableCache) { return false; }

        // 如果缓存标志有效返回 true
        $cacheFile = $this->_getCacheFile($file, $cacheId);
        if (isset($this->cacheState[$cacheFile]) && $this->cacheState[$cacheFile]) {
            return true;
        }

        // 检查缓存文件是否存在
        if (!is_readable($cacheFile)) { return false; }

        // 检查缓存文件是否已经过期
        $mtime = filemtime($cacheFile);
        if ($mtime == false) { return false; }
        if (($mtime + $this->cacheLifetime) < time()) {
            $this->cacheState[$cacheFile] = false;
            @unlink($cacheFile);
            return false;
        }

        $this->cacheState[$cacheFile] = true;
        return true;
    }

    /**
     * 清除指定的缓存
     *
     * @param string $file 模板资源名
     * @param string $cacheId 缓存 ID
     */
    function cleanCache($file, $cacheId = null) {
        @unlink($this->_getCacheFile($file, $cacheId));
    }

    /**
     * 清除所有缓存
     */
    function cleanAllCache() {
        foreach (glob($this->cacheDir . '/' . "*.php") as $filename) {
            @unlink($filename);
        }
    }

    /**
     * 返回缓存文件名
     *
     * @param string $file
     * @param string $cacheId
     *
     * @return string
     */
    function _getCacheFile($file, $cacheId) {
        return $this->cacheDir . DIRECTORY_SEPARATOR . rawurlencode($file . '-' . $cacheId) . '.php';
    }
}
