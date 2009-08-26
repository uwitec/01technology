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
 * 定义 Generator 类
 *
 * 该脚本入口完成运行环境初始化、调用参数解析等工作
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Scripts
 * @version $Id: Generator.php 1000 2007-10-30 05:40:53Z qeeyuan $
 */

/**
 * Generator 类是所有生成器的入口
 *
 * @package Tool
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class Generator
{
    /**
     * 保存命令行参数
     *
     * @var array
     */
    var $_argv;
    
    var $_generatorList = array(
        'controller' => array(
            'class' => 'Generator_Controller',
            'help'  => '<controller name> [controller class name]',
        ),
        'table'      => array(
            'class' => 'Generator_Table',
            'help'  => '<database table name> [table class name]',
        ),
        'model'      => array(
            'class' => 'Generator_Model',
            'help'  => '<class name> <database table name> [table class name]',
        ),
    );
    
    function Generator($argv)
    {
        $this->_argv = $argv;
        FLEA::import(dirname(__FILE__));
    }
    
    function run()
    {
        if (count($this->_argv) < 2) {
            $this->help();
        }
        
        $type = strtolower($this->_argv[1]);
        if (!isset($this->_generatorList[$type])) {
            $this->help();
        } else {
            $class = $this->_generatorList[$type]['class'];
            $generator =& FLEA::getSingleton($class);
            /* @var $generator Generator_Abstract */
            $argv = $this->_argv;
            array_shift($argv);
            array_shift($argv);
            $generator->generate($argv);
        }
    }
    
    function help()
    {
        echo <<<EOT

    php generator.php <type> <....>

example:

EOT;

        foreach ($this->_generatorList as $type => $generator) {
            echo "    php generate.php {$type} {$generator['help']}\n";
        }
        echo "\n";

        exit(-1);
    }
}
