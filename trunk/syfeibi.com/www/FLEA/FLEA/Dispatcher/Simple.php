<?php
/////////////////////////////////////////////////////////////////////////////
// FleaPHP Framework
//
// Copyright (c) 2005 - 2007 FleaPHP.org (www.fleaphp.org)
//
// 许可协议，请查看源代码中附带的 LICENSE.txt 文件，
// 或者访问 http://www.fleaphp.org/ 获得详细信息。
/////////////////////////////////////////////////////////////////////////////

/**
 * 定义 FLEA_Dispatcher_Simple 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Simple.php 1028 2008-02-02 05:50:59Z qeeyuan $
 */

/**
 * FLEA_Dispatcher_Simple 分析 HTTP 请求，并转发到合适的 Controller 对象处理
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Dispatcher_Simple
{
    /**
     * 保存了请求信息的数组
     *
     * @var array
     */
    var $_request;

    /**
     * 原始的请求信息数组
     *
     * @var array
     */
    var $_requestBackup;

    /**
     * 构造函数
     *
     * @param array $request
     *
     * @return FLEA_Dispatcher_Simple
     */
    function FLEA_Dispatcher_Simple(& $request)
    {
        $this->_requestBackup =& $request;

        $controllerAccessor = strtolower(FLEA::getAppInf('controllerAccessor'));
        $actionAccessor = strtolower(FLEA::getAppInf('actionAccessor'));

        $r = array_change_key_case($request, CASE_LOWER);
        $data = array('controller' => null, 'action' => null);
        if (isset($r[$controllerAccessor])) {
            $data['controller'] = $r[$controllerAccessor];
        }
        if (isset($r[$actionAccessor])) {
            $data['action'] = $r[$actionAccessor];
        }
        $this->_request = $data;
    }

    /**
     * 从请求中分析 Controller、Action 和 Package 名字，然后执行指定的 Action 方法
     *
     * @return mixed
     */
    function dispatching()
    {
        $controllerName = $this->getControllerName();
        $actionName = $this->getActionName();
        return $this->_executeAction($controllerName, $actionName, $this->getControllerClass($controllerName));
    }

    /**
     * 执行指定的 Action 方法
     *
     * @param string $controllerName
     * @param string $actionName
     * @param string $controllerClass
     *
     * @return mixed
     */
    function _executeAction($controllerName, $actionName, $controllerClass)
    {
        $callback = FLEA::getAppInf('dispatcherFailedCallback');

        // 确定动作方法名
        $actionPrefix = FLEA::getAppInf('actionMethodPrefix');
        $actionMethod = $actionPrefix . $actionName . FLEA::getAppInf('actionMethodSuffix');

        $controller = null;
        $controllerClassFilename = null;
        do {
            // 载入控制对应的类定义
            if (!$this->_loadController($controllerClass)) { break; }

            // 构造控制器对象
            FLEA::setAppInf('FLEA.internal.currentControllerName', $controllerName);
            FLEA::setAppInf('FLEA.internal.currentActionName', $actionName);
            $controller =& new $controllerClass($controllerName);
            if (!method_exists($controller, $actionMethod)) { break; }
            if (method_exists($controller, '__setController')) {
                $controller->__setController($controllerName, $actionName);
            }
            if (method_exists($controller, '__setDispatcher')) {
                $controller->__setDispatcher($this);
            }

            // 调用 _beforeExecute() 方法
            if (method_exists($controller, '_beforeExecute')) {
                $controller->_beforeExecute($actionMethod);
            }
            // 执行 action 方法
            $ret = $controller->{$actionMethod}();
            // 调用 _afterExecute() 方法
            if (method_exists($controller, '_afterExecute')) {
                $controller->_afterExecute($actionMethod);
            }
            return $ret;
        } while (false);

        if ($callback) {
            // 检查是否调用应用程序设置的错误处理程序
            $args = array($controllerName, $actionName, $controllerClass);
            return call_user_func_array($callback, $args);
        }

        if (is_null($controller)) {
            FLEA::loadClass('FLEA_Exception_MissingController');
            __THROW(new FLEA_Exception_MissingController(
                    $controllerName, $actionName, $this->_requestBackup,
                    $controllerClass, $actionMethod, $controllerClassFilename));
            return false;
        }

        FLEA::loadClass('FLEA_Exception_MissingAction');
        __THROW(new FLEA_Exception_MissingAction(
                $controllerName, $actionName, $this->_requestBackup,
                $controllerClass, $actionMethod, $controllerClassFilename));
        return false;
    }

    /**
     * 从请求中取得 Controller 名字
     *
     * 如果没有指定 Controller 名字，则返回配置文件中定义的默认 Controller 名字。
     *
     * @return string
     */
    function getControllerName()
    {
        $controllerName = preg_replace('/[^a-z0-9_]+/i', '', $this->_request['controller']);
        if ($controllerName == '') {
            $controllerName = FLEA::getAppInf('defaultController');
        }
        if (FLEA::getAppInf('urlLowerChar')) {
            $controllerName = strtolower($controllerName);
        }
        return $controllerName;
    }

    /**
     * 设置要访问的控制器名字
     *
     * @param string $controllerName
     */
    function setControllerName($controllerName)
    {
        $this->_request['controller'] = $controllerName;
    }

    /**
     * 从请求中取得 Action 名字
     *
     * 如果没有指定 Action 名字，则返回配置文件中定义的默认 Action 名字。
     *
     * @return string
     */
    function getActionName()
    {
        $actionName = preg_replace('/[^a-z0-9]+/i', '', $this->_request['action']);
        if ($actionName == '') {
            $actionName = FLEA::getAppInf('defaultAction');
        }
        return $actionName;
    }

    /**
     * 设置要访问的动作名字
     *
     * @param string $actionName
     */
    function setActionName($actionName)
    {
        $this->_request['action'] = $actionName;
    }

    /**
     * 返回指定控制器对应的类名称
     *
     * @param string $controllerName
     *
     * @return string
     */
    function getControllerClass($controllerName)
    {
        $controllerClass = FLEA::getAppInf('controllerClassPrefix');
        if (FLEA::getAppInf('urlLowerChar')) {
            $controllerClass .= ucfirst(strtolower($controllerName));
        } else {
            $controllerClass .= $controllerName;
        }
        return $controllerClass;
    }

    /**
     * 分析 url 地址，找出控制器名字和动作名
     *
     * @param string $url
     *
     * @return array
     */
    function parseUrl($url)
    {
        $url = parse_url($url);
        $args = array();
        parse_str($url['query'], $args);
        $args = array_change_key_case($args, CASE_LOWER);
        $controllerAccessor = strtolower(FLEA::getAppInf('controllerAccessor'));
        $actionAccessor = strtolower(FLEA::getAppInf('actionAccessor'));

        $controllerName = isset($args[$controllerAccessor]) ?
                $args[$controllerAccessor] : null;
        $actionName = isset($args[$actionAccessor]) ?
                $args[$actionAccessor] : null;

        unset($args[$controllerAccessor]);
        unset($args[$actionAccessor]);
        return array($controllerName, $actionName, $args);
    }

    /**
     * 载入控制器类
     *
     * @param string $controllerClass
     *
     * @return boolean
     */
    function _loadController($controllerClass)
    {
        $controllerClassFilename = FLEA::getFilePath($controllerClass . '.php', true);
        if (!is_readable($controllerClassFilename)) {
            return false;
        }
        include_once($controllerClassFilename);
        return class_exists($controllerClass);
    }
}
