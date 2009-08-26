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
 * 该文件定义了一些用户简化文件系统操作的函数和对象
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: FileSystem.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * 创建一个目录树
 *
 * 用法：
 * <code>
 * mkdirs('/top/second/3rd');
 * </code>
 *
 * @param string $dir
 * @param int $mode
 */
function mkdirs($dir, $mode = 0777)
{
    if (!is_dir($dir)) {
        mkdirs(dirname($dir), $mode);
        return mkdir($dir, $mode);
    }
    return true;
}

/**
 * 删除指定目录及其下的所有文件和子目录
 *
 * 用法：
 * <code>
 * // 删除 my_dir 目录及其下的所有文件和子目录
 * rmdirs('/path/to/my_dir');
 * </code>
 *
 * 注意：使用该函数要非常非常小心，避免意外删除重要文件。
 *
 * @param string $dir
 */
function rmdirs($dir)
{
    $dir = realpath($dir);
    if ($dir == '' || $dir == '/' ||
        (strlen($dir) == 3 && substr($dir, 1) == ':\\'))
    {
        // 禁止删除根目录
        return false;
    }

    // 遍历目录，删除所有文件和子目录
    if(false !== ($dh = opendir($dir))) {
        while(false !== ($file = readdir($dh))) {
            if($file == '.' || $file == '..') { continue; }
            $path = $dir . DIRECTORY_SEPARATOR . $file;
            if (is_dir($path)) {
                if (!rmdirs($path)) { return false; }
            } else {
                unlink($path);
            }
        }
        closedir($dh);
        rmdir($dir);
        return true;
    } else {
        return false;
    }
}
