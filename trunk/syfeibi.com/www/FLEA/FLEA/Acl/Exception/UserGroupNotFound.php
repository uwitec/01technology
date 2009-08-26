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
 * 定义 FLEA_Acl_Exception_UserGroupNotFound 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: UserGroupNotFound.php 1060 2008-05-04 05:02:59Z qeeyuan $
 */

/**
 * FLEA_Acl_Exception_UserGroupNotFound 指示指定的用户组没有找到
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Acl_Exception_UserGroupNotFound extends FLEA_Exception
{
    var $userGroupId;

    function FLEA_Acl_Exception_UserGroupNotFound($userGroupId)
    {
        $this->userGroupId = $userGroupId;
        parent::FLEA_Exception("UserGroup ID: {$userGroupId} not found.");
    }
}
