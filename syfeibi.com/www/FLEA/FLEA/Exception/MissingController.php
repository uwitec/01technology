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
 * 定义 FLEA_Exception_MissingController 异常
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Exception
 * @version $Id: MissingController.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Exception_MissingController 指示请求的控制器没有找到
 *
 * @package Exception
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Exception_MissingController extends FLEA_Exception
{
    /**
     * 控制器的名字
     *
     * @var string
     */
    var $controllerName;

    /**
     * 控制器类名称
     *
     * @var string
     */
    var $controllerClass;

    /**
     * 动作名
     *
     * @var string
     */
    var $actionName;

    /**
     * 动作方法名
     *
     * @var string
     */
    var $actionMethod;

    /**
     * 调用参数
     *
     * @var mixed
     */
    var $arguments;

    /**
     * 控制器的类定义文件
     *
     * @var string
     */
    var $controllerClassFilename;

    /**
     * 构造函数
     *
     * @param string $controllerName
     * @param string $actionName
     * @param mixed $arguments
     * @param string $controllerClass
     * @param string $actionMethod
     *
     * @return FLEA_Exception_MissingController
     */
    function FLEA_Exception_MissingController($controllerName, $actionName,
             $arguments = null, $controllerClass = null, $actionMethod = null,
             $controllerClassFilename = null)
    {
        $this->controllerName = $controllerName;
        $this->actionName = $actionName;
        $this->arguments = $arguments;
        $this->controllerClass = $controllerClass;
        $this->actionMethod = $actionMethod;
        $this->controllerClassFilename = $controllerClassFilename;
        $code = 0x0103002;
        parent::FLEA_Exception(sprintf(_ET($code), $controllerName));
    }
}
