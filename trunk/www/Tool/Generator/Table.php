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
 * 定义 Generator_Table 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Scripts
 * @version $Id: Table.php 1000 2007-10-30 05:40:53Z qeeyuan $
 */

// {{{ includes
require_once dirname(__FILE__) . '/Abstract.php';
// }}}

/**
 * Generator_Table 根据应用程序的数据库设置创建需要的表数据入口对象定义文件
 *
 * @package Scripts
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class Generator_Table extends Generator_Abstract
{
    /**
     * 执行生成器
     *
     * @param array $opts
     */
    function generate($opts)
    {
        $name = array_shift($opts);
        if (!empty($opts[0])) {
            $tableClass = $opts[0];
        } else {
            $tableClass = 'Table_' . ucfirst($this->_camelName($name));
        }

        if ($filename = $this->_existsClassFile($tableClass)) {
            echo "Class '{$tableClass}' declare file '{$filename}' exists.\n";
            return -1;
        }

        $content = $this->_getCode($name, $tableClass);
        if ($content !== -1 && !empty($content)) {
            return $this->_createClassFile($tableClass, $content);
        } else {
            return -1;
        }
    }

    /**
     * 生成代码
     *
     * @param string $tableName
     * @param string $tableClass
     *
     * @return string
     */
    function _getCode($tableName, $tableClass)
    {
        /**
         * 首先判断指定的数据表是否存在
         */
        $dbo =& FLEA::getDBO();
        $tables = $dbo->metaTables();
        if (!in_array($tableName, $tables)) {
            echo "Database table '{$tableName}' not exists.\n";
            return -1;
        }
        $meta = $dbo->metaColumns($tableName);
        $primaryKey = '';
        foreach ($meta as $field) {
            if ($field['primaryKey']) {
                $primaryKey = $field['name'];
                break;
            }
        }

        $viewdata = array(
            'tableName'     => $tableName,
            'tableClass'    => $tableClass,
            'primaryKey'    => $primaryKey,
        );
        return $this->_parseTemplate('table', $viewdata);
    }
}