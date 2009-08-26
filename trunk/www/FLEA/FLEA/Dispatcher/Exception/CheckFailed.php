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
 * 定义 FLEA_Dispatcher_Exception_CheckFailed 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: CheckFailed.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Dispatcher_Exception_CheckFailed 异常指示用户试图访问的控制器方法不允许该用户访问
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Dispatcher_Exception_CheckFailed extends FLEA_Exception
{
    var $controllerName;
    var $actionName;
    var $roles;
    var $act;

    /**
     * 构造函数
     *
     * @param string $controllerName
     * @param string $actionName
     * @param array $act
     * @param array $roles
     *
     * @return FLEA_Dispatcher_Exception_CheckFailed
     */
    function FLEA_Dispatcher_Exception_CheckFailed($controllerName, $actionName,
            $act = null, $roles = null)
    {
        $this->controllerName = $controllerName;
        $this->actionName = $actionName;
        $this->act = $act;
        $this->roles = $roles;
        $code = 0x0701004;
        $msg = sprintf(_ET($code), $controllerName, $actionName);
        parent::FLEA_Exception($msg, $code);
    }
}
