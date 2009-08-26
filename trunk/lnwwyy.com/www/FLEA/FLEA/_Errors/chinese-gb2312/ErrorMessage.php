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
 * 定义 FleaPHP 中所有错误信息的代码及对应的错误信息
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: ErrorMessage.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

return array(
    // FLEA_Exception_ExpectedFile
    0x0102001 => '需要的文件 "%s" 没有找到.',
    // FLEA_Exception_ExpectedClass
    0x0102002 => '文件 "%s" 中没有定义需要的类 "%s".',
    0x0102003 => '没有定义需要的类 "%s".',
    // FLEA_Exception_ExistsKeyName
    0x0102004 => '键名 "%s" 已经存在.',
    // FLEA_Exception_FileOperation
    0x0102005 => '文件系统操作 "%s" 失败.',
    // FLEA_Exception_InvalidArguments
    0x0102006 => '无效的参数 "%s".',
    // FLEA_Exception_MissingArguments
    0x0102007 => '没有提供需要的 "%s" 参数.',
    // FLEA_Exception_MustOverwrite
    0x0102008 => '必须在继承类中覆盖类方法 "%s".',
    // FLEA_Exception_NotExistsKeyName
    0x0102009 => '需要的键名 "%s" 不存在.',
    // FLEA_Exception_NotImplemented
    0x010200a => '类方法 "%s::%s" 没有实现.',
    0x010200b => '函数 "%s" 没有实现.',
    // FLEA_Exception_TypeMismatch
    0x010200c => '参数 "%s" 预期的类型是 "%s", 但其实际类型为 "%s".',
    // FLEA_Exception_CacheDisabled
    0x010200d => '缓存功能被禁用，通常这是因为没有指定应用程序设置 internalCacheDir 选项，者 internalCacheDir 设置指定的目录不可写入文件.',

    // FLEA_Exception_MissingAction
    0x0103001 => '缺少控制器方法 "%s::%s()".',
    // FLEA_Exception_MissingController
    0x0103002 => '缺少控制器 "%s".',

    // FLEA_Exception_ValidationFailed
    0x0407001 => '下列数据验证失败: "%s".',

    // FLEA_Com_RBAC_Exception_InvalidACT
    0x0701001 => '无效的访问控制表“Access-Control-Table (ACT)”数据.',
    // FLEA_Com_RBAC_Exception_InvalidACTFile
    0x0701002 => '控制器 "%s" 使用了无效的访问控制表“Access-Control-Table (ACT)”文件.',
    0x0701003 => '无效的访问控制表“Access-Control-Table (ACT)”文件.',
    // FLEA_Dispatcher_Exception_CheckFailed
    0x0701004 => '拒绝访问控制器动作: "%s::%s".',
    // Warning
    0x0701005 => '应用程序使用了默认的 RBACSessionKey 设置.',
    // Warning
    0x0701006 => '没有找到控制器 \'%s\' 的 ACT 文件.',

    // FLEA_Db_Exception_InvalidDSN
    0x06ff001 => '无效的数据源名称“Data-Source-Name (DSN)”.',
    // FLEA_Db_Exception_MissingDSN
    0x06ff002 => '缺少应用程序设置: dbDSN.',
    // FLEA_Db_Exception_MissingPrimaryKey
    0x06ff003 => '缺少主键 "%s" 的值.',
    // FLEA_Db_Exception_PrimaryKeyExists
    0x06ff004 => '主键值 "%s" = "%s" 已经存在.',
    // FLEA_Db_Exception_SqlQuery
    0x06ff005 => "SQL 错误消息: \"%s\"\nSQL 语句: \"%s\"\nSQL 错误代码: \"%s\".",
    0x06ff006 => "SQL 语句: \"%s\"\nSQL 错误代码: \"%s\".",
    // FLEA_Db_Exception_MetaColumnsFailed
    0x06ff007 => "获取数据表 \"%s\" 的元数据时发生错误.",
    // FLEA_Db_Exception_InvalidInsertID
    0x06ff008 => "无法获取最后插入记录的主键值.",
    // FLEA_Db_Exception_MissingLink
    0x06ff009 => "指定的关联对象 \"%s\" 不存在.",

    // FLEA_Db_Exception_InvalidLinkType
    0x0202001 => '无效的数据表关联类型 "%s".',
    // FLEA_Db_Exception_MissingLinkOption
    0x0202002 => '创建数据表关联时缺少必须的 "%s" 选项.',

    // FLEA_View_Exception_NotConfigurationSmarty
    0x0902001 => 'FLEA_View_Smarty 需要应用程序设置 "viewConfig[\'smartyDir\']"，并定义常量 SMARTY_DIR.',
    // FLEA_View_Exception_InitSmartyFailed
    0x0902002 => 'Smarty 模板引擎文件 "%s" 没有找到.',

    // FLEA_View_Exception_NotConfigurationSmartTemplate
    0x0903001 => 'FLEA_View_SmartTemplate 需要应用程序设置 "viewConfig[\'smartDir\']".',
    // FLEA_View_Exception_InitSmartTemplateFailed
    0x0903002 => 'SmartTemplate 模板引擎文件 "%s" 没有找到.',


    // FLEA_View_Exception_NotConfigurationLite
    0x0904001 => 'FLEA_View_Lite 需要应用程序设置 "viewConfig[\'liteDir\']".',
    // FLEA_View_Exception_InitLiteFailed
    0x0904002 => 'TemplateLite 模板引擎文件 "%s" 没有找到.',
);
