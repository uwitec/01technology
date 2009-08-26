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
 * 定义 Generator_Model 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Scripts
 * @version $Id: Model.php 1000 2007-10-30 05:40:53Z qeeyuan $
 */

// {{{ includes
require_once dirname(__FILE__) . '/Abstract.php';
// }}}

/**
 * Generator_Model 根据应用程序的数据库设置创建需要的 ActiveRecord 对象定义文件
 *
 * @package Scripts
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class Generator_Model extends Generator_Abstract
{
    /**
     * 执行生成器
     *
     * @param array $opts
     */
    function generate($opts)
    {
        $name = array_shift($opts);
        $modelClass = 'Model_' . ucfirst($name);
        if ($filename = $this->_existsClassFile($modelClass)) {
            echo "Class '{$modelClass}' declare file '{$filename}' exists.\n";
            return -1;
        }

        $tableName = reset($opts);
        if (empty($tableName)) { return -1; }

        if (isset($opts[1])) {
            $tableClass = $opts[1];
        } else {
            $tableClass = 'Table_' . ucfirst($this->_camelName($tableName));
        }

        /**
         * 首先判断需要的表数据入口对象是否存在
         */
        if (!$this->_existsClassFile($tableClass)) {
            /**
             * 创建需要的表数据入口对象
             */
            $generatorTable =& FLEA::getSingleton('Generator_Table');
            /* @var $generatorTable Generator_Table */
            $generatorTable->make($tableName, array($tableClass));
        }

        FLEA::loadClass('FLEA_Db_TableDataGateway');
        $table =& FLEA::getSingleton($tableClass);

        $content = $this->_getCode($modelClass, $tableClass, $table);
        if ($content !== -1 && !empty($content)) {
            return $this->_createClassFile($modelClass, $content);
        } else {
            return -1;
        }
    }

    /**
     * 生成代码
     *
     * @param string $modelClass
     * @param string $tableClass
     * @param FLEA_Db_TableDataGateway $table
     *
     * @return string
     */
    function _getCode($modelClass, $tableClass, & $table)
    {
        static $typeMap = array(
            'C' => 'string',
            'X' => 'string',
            'B' => 'string',
            'N' => 'float',
            'D' => 'string',
            'T' => 'int',
            'L' => 'boolean',
            'I' => 'int',
            'R' => 'int',
        );

        $propertiesMapping = array();
        $len = 0;
        $propPrimaryKey = null;
        foreach ($table->meta as $field) {
            $prop = $this->_camelName($field['name']);
            $len = strlen($prop) > $len ? strlen($prop) : $len;
            $field['phpType'] = $typeMap[$field['simpleType']];
            $propertiesMapping[$prop] = $field;
            if ($field['simpleType'] == 'R') {
                $propPrimaryKey = $prop;
            }
        }

        $viewdata = array(
            'modelClass' => $modelClass,
            'tableClass' => $tableClass,
            'mapping'    => $propertiesMapping,
            'len'        => $len,
        );

        return $this->_parseTemplate('model', $viewdata);
    }
}
