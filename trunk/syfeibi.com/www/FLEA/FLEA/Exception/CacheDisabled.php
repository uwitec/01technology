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
 * 定义 FLEA_Exception_CacheDisabled 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: CacheDisabled.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Exception_CacheDisabled 异常指示缓存功能被禁用
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Exception_CacheDisabled extends FLEA_Exception
{
    /**
     * 缓存目录
     */
    var $cacheDir;

    /**
     * 构造函数
     *
     * @param string $cacheDir
     *
     * @return FLEA_Exception_CacheDisabled
     */
    function FLEA_Exception_CacheDisabled($cacheDir)
    {
        $this->cacheDir = $cacheDir;
        parent::FLEA_Exception(_ET(0x010200d));
    }
}
