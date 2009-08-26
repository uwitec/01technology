<?php
// 载入基础类的定义
FLEA::loadClass('FLEA_Db_TableDataGateway');

/**
 * Model_Category 类是 FLEA_Db_TableDataGateway 类的一个子类。
 *
 * 通过指定 $tableName 和 $primaryKey 属性，就能够用 Model_Category 对数据表进行
 * CRUD（创建、读取、更新、删除）操作，而无需编写数据库操作代码，提供了开发效率。
 */
class Model_Category extends FLEA_Db_TableDataGateway
{
    /**
     * $tableName 属性用于指定 Model_Category 是操作哪一个数据表
     *
     * @var string
     */
    var $tableName = 'category';
    // 指定主键字段名

    /**
     * $primaryKey 属性指定要操作的数据表的主键字段名
     *
     * @var string
     */
    var $primaryKey = 'cateid';
    
	var $hasMany = array(
		'tableClass' => 'Model_Goods',
		'mappingName' => 'goods',
	);
}
