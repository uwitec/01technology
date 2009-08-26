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
 * 定义 FLEA_Com_RBAC_Exception_InvalidACTFile 异常，是 FLEA_Rbac_Exception_InvalidACTFile 的别名
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: InvalidACTFile.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

FLEA::loadClass('FLEA_Rbac_Exception_InvalidACTFile');

/**
 * 开发者应该使用 FLEA_Rbac_Exception_InvalidACTFile
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Com_RBAC_Exception_InvalidACTFile extends FLEA_Rbac_Exception_InvalidACTFile
{
}
