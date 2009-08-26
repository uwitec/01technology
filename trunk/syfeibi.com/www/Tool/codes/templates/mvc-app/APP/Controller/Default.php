<?php

// 确保需要的类定义文件被载入
FLEA::loadClass('FLEA_Controller_Action');

/**
 * 应用程序的默认控制器
 */
class Controller_Default extends FLEA_Controller_Action
{
    /**
     * 默认动作
     */
    function actionIndex()
    {
        // 准备模板需要的数据
        $viewdata = array(
            'code_filename' => __FILE__,
        );

        // 显示模板
        // 模板文件名通常按照“控制器名_动作名”的方式命名，便于确定模板用途
        $this->_executeView(APP_DIR . '/View/default_index.php', $viewdata);
    }
}
