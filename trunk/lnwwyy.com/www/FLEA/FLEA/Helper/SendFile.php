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
 * 定义 FLEA_Helper_SendFile 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: SendFile.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

// {{{ constants
define('SENDFILE_ATTACHMENT', 'attachment');
define('SENDFILE_INLINE', 'inline');
// }}}

/**
 * FLEA_Helper_SendFile 类用于向浏览器发送文件
 *
 * 利用 FLEA_Helper_SendFile，应用程序可以将重要的文件保存在
 * 浏览器无法访问的位置。然后通过程序将文件内容发送给浏览器。
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Helper_SendFile
{
    /**
     * 向浏览器发送文件内容
     *
     * @param string $serverPath 文件在服务器上的路径（绝对或者相对路径）
     * @param string $filename 发送给浏览器的文件名（尽可能不要使用中文）
     * @param string $mimeType 指示文件类型
     */
    function sendFile($serverPath, $filename, $mimeType = 'application/octet-stream')
    {
        header("Content-Type: {$mimeType}");
        $filename = '"' . htmlspecialchars($filename) . '"';
        $filesize = filesize($serverPath);
        $charset = FLEA::getAppInf('responseCharset');
        header("Content-Disposition: attachment; filename={$filename}; charset={$charset}");
        header('Pragma: cache');
        header('Cache-Control: public, must-revalidate, max-age=0');
        header("Content-Length: {$filesize}");
        readfile($serverPath);
        exit;
    }
}
