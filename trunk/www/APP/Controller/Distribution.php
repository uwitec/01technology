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
 * 定义 Controller_Default 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Example
 * @subpackage Smarty
 * @version $Id: Default.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * Controller_Default 类是 Smarty 示例的默认控制器
 *
 * @package Example
 * @subpackage Smarty
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class Controller_Distribution extends FLEA_Controller_Action
{
    /**
     * Model_News 的实例
     *
     * Model_News 对象提供了 News 数据的查询、添加、删除和更新等操作。
     *
     * @var Model_News
     */
    var $_modelDistribution;
    var $_metas;

    /**
     * 构造函数
     *
     * @return Controller_Default
     */
    function Controller_Distribution()
    {
        /**
         * FLEA::getSingleton() 会自动载入指定类的定义文件，并且返回该类的唯一一个实例
         */
        //$this->_modelAbout =& FLEA::getSingleton('Model_Message');
        $this->_metas =& FLEA::getAppInf('metas');
		$this->_metas = FLEA::getAppInf('metas');
		$this->_metas['description'] = "辽宁万维医药经销渠道面向东三省招商";
		$this->_metas['keywords'] = "辽宁万维医药,辽宁,吉林省,黑龙江省,招商";
		$this->_metas['title'] = $this->_metas['title'] . "经销网络";        
    }
	
    /**
     * 列出所有信息
     *
     */
/*    function actionList() {
    	$news = $this->_modelAbout->findAll();
    	echo "<pre>";
    	print_r($news);
    	exit;
    }*/
    
	/**
     * 默认控制器方法
     */
    function actionIndex() {

        $smarty =& $this->_getView();
        /* @var $smarty Smarty */
        //$smarty->assign('my_var', 'The smarty template engine.');
        $smarty->assign('metas', $this->_metas);
        $smarty->display('tpl-distribution.html');
    }

    
    /**
     * 演示另一种使用 Smarty 的方法
     */
    function actionAlternative() {
        $viewData = array(
            'my_var' => 'The smarty template engine.',
        );
        $this->_executeView('tpl-alternative.html', $viewData);
    }
}
