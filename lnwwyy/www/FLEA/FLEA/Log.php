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
 * 定义 FLEA_Log 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Log.php 999 2007-10-30 05:39:57Z qeeyuan $
 */

/**
 * 追加日志记录
 *
 * @param string $msg
 * @param string $level
 */
function log_message($msg, $level = 'log', $title = '')
{
    static $instance = null;

    if (is_null($instance)) {
        $instance = array();
        $obj =& FLEA::getSingleton('FLEA_Log');
        $instance = array('obj' => & $obj);
    }

    return $instance['obj']->appendLog($msg, $level, $title);
}

/**
 * FLEA_Log 类提供基本的日志服务
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Log
{
    /**
     * 保存运行期间的日志，在教本结束时将日志内容写入到文件
     *
     * @var string
     */
    var $_log = '';

    /**
     * 日期格式
     *
     * @var string
     */
    var $dateFormat = 'Y-m-d H:i:s';

    /**
     * 保存日志文件的目录
     *
     * @var string
     */
    var $_logFileDir;

    /**
     * 保存日志的文件名
     *
     * @var string
     */
    var $_logFilename;

    /**
     * 是否允许日志保存
     *
     * @var boolean
     */
    var $_enabled = true;

    /**
     * 要写入日志文件的错误级别
     *
     * @var array
     */
    var $_errorLevel;

    /**
     * 构造函数
     *
     * @return FLEA_Log
     */
    function FLEA_Log()
    {
        $dir = FLEA::getAppInf('logFileDir');
        if (empty($dir)) {
            // 如果没有指定日志存放目录，则保存到内部缓存目录中
            $dir = FLEA::getAppInf('internalCacheDir');
        }
        $dir = realpath($dir);
        if (substr($dir, -1) != DIRECTORY_SEPARATOR) {
            $dir .= DIRECTORY_SEPARATOR;
        }
        if (!is_dir($dir) || !is_writable($dir)) {
            $this->_enabled = false;
        } else {
            $this->_logFileDir = $dir;
            $this->_logFilename = $this->_logFileDir . FLEA::getAppInf('logFilename');
            $errorLevel = explode(',', strtolower(FLEA::getAppInf('logErrorLevel')));
            $errorLevel = array_map('trim', $errorLevel);
            $errorLevel = array_filter($errorLevel, 'trim');
            $this->_errorLevel = array();
            foreach ($errorLevel as $e) {
               $this->_errorLevel[$e] = true;
            }

            global $___fleaphp_loaded_time;
            list($usec, $sec) = explode(" ", $___fleaphp_loaded_time);
            $this->_log = sprintf("[%s %s] ======= FleaPHP Loaded =======\n",
                date($this->dateFormat, $sec), $usec);

            if (isset($_SERVER['REQUEST_URI'])) {
                $this->_log .= sprintf("[%s] REQUEST_URI: %s\n",
                        date($this->dateFormat),
                        $_SERVER['REQUEST_URI']);
            }

            // 注册脚本结束时要运行的方法，将缓存的日志内容写入文件
            register_shutdown_function(array(& $this, '__writeLog'));

            // 检查文件是否已经超过指定大小
            if (file_exists($this->_logFilename)) {
                $filesize = filesize($this->_logFilename);
            } else {
                $filesize = 0;
            }
            $maxsize = (int)FLEA::getAppInf('logFileMaxSize');
            if ($maxsize >= 512) {
                $maxsize = $maxsize * 1024;
                if ($filesize >= $maxsize) {
                    // 使用新的日志文件名
                    $pathinfo = pathinfo($this->_logFilename);
                    $newFilename = $pathinfo['dirname'] . DS .
                        basename($pathinfo['basename'], '.' . $pathinfo['extension']) .
                        date('-Ymd-His') . '.' . $pathinfo['extension'];
                    rename($this->_logFilename, $newFilename);
                }
            }
        }
    }

    /**
     * 追加日志信息
     *
     * @param string $msg
     * @param string $level
     */
    function appendLog($msg, $level = 'log', $title = '')
    {
        if (!$this->_enabled) { return; }
        $level = strtolower($level);
        if (!isset($this->_errorLevel[$level])) { return; }

        $msg = sprintf("[%s] [%s] %s:%s\n", date($this->dateFormat), $level, $title, print_r($msg, true));
        $this->_log .= $msg;
    }

    /**
     * 将日志信息写入缓存
     */
    function __writeLog()
    {
        global $___fleaphp_loaded_time;

        // 计算应用程序执行时间（不包含入口文件）
        list($usec, $sec) = explode(" ", $___fleaphp_loaded_time);
        $beginTime = (float)$sec + (float)$usec;
        $endTime = microtime();
        list($usec, $sec) = explode(" ", $endTime);
        $endTime = (float)$sec + (float)$usec;
        $elapsedTime = $endTime - $beginTime;
        $this->_log .= sprintf("[%s %s] ======= FleaPHP End (elapsed: %f seconds) =======\n\n",
            date($this->dateFormat, $sec), $usec, $elapsedTime);

        $fp = fopen($this->_logFilename, 'a');
        if (!$fp) { return; }
        flock($fp, LOCK_EX);
        fwrite($fp, str_replace("\r", '', $this->_log));
        flock($fp, LOCK_UN);
        fclose($fp);
    }
}
