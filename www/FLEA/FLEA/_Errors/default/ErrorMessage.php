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
    0x0102001 => 'Required file "%s" is missing.',
    // FLEA_Exception_ExpectedClass
    0x0102002 => 'Required class "%s" can not be found in file "%s."',
    0x0102003 => 'Required class "$s" is not defined.',
    // FLEA_Exception_ExistsKeyName
    0x0102004 => 'Key "%s" already exists.',
    // FLEA_Exception_FileOperation
    0x0102005 => 'Failed to execute file operation "%s".',
    // FLEA_Exception_InvalidArguments
    0x0102006 => 'Invalid parameter "%s".',
    // FLEA_Exception_MissingArguments
    0x0102007 => 'Required parameter "%s" is not provided.',
    // FLEA_Exception_MustOverwrite
    0x0102008 => 'Method "%s" must be overwritten in extended class.',
    // FLEA_Exception_NotExistsKeyName
    0x0102009 => 'Required key "%s" does not exist.',
    // FLEA_Exception_NotImplemented
    0x010200a => 'Class method "$s::$s" is empty.',
    0x010200b => 'Function "%s" is empty.',
    // FLEA_Exception_TypeMismatch
    0x010200c => 'The data type of parameter "%s" is "%s", but expected to be "%s".',
    // FLEA_Exception_CacheDisabled
    0x010200d => 'Cache function is disabled now. This is usually because internalCacheDir option is not defined or internalCacheDir directory is unwritable',

    // FLEA_Exception_MissingAction
    0x0103001 => 'Controller method "%s::%s()" is missing.',
    // FLEA_Exception_MissingController
    0x0103002 => 'Controller "%s" is missing.',

    // FLEA_Exception_ValidationFailed
    0x0407001 => 'The following data is invalid: "%s".',

    // FLEA_Rbac_Exception_InvalidACT
    0x0701001 => 'Invalid "Access-Control-Table (ACT)".',
    // FLEA_Rbac_Exception_InvalidACTFile
    0x0701002 => 'Invalid Access-Controll-Table(ACT) was used by controller "%s".',
    0x0701003 => 'Invalid "Access-Control-Table (ACT)" file.',
    // FLEA_Dispatcher_Exception_CheckFailed
    0x0701004 => 'Access to controller action "%s::%s" is denied.',
    // Warning
    0x0701005 => 'Default RBACSessionKey is used now.',
    // Warning
    0x0701006 => 'ACT file \'%s\' is not found.',

    // FLEA_Db_Exception_InvalidDSN
    0x06ff001 => 'Invalid DSN(Data-Source-Name).',
    // FLEA_Db_Exception_MissingDSN
    0x06ff002 => 'dbDSN has to be set a value.',
    // FLEA_Db_Exception_MissingPrimaryKey
    0x06ff003 => 'The value of primary key "%s" is missing.',
    // FLEA_Db_Exception_PrimaryKeyExists
    0x06ff004 => 'Primary key "%s" = "%s" already exists.',
    // FLEA_Db_Exception_SqlQuery
    0x06ff005 => "SQL Error Message: \"%s\"\nSQL : \"%s\"\nSQL Error code: \"%s\".",
    0x06ff006 => "SQL Script: \"%s\"\nSQL Error Code: \"%s\".",
    // FLEA_Db_Exception_MetaColumnsFailed
    0x06ff007 => "Failed to get meta data from table \"%s\".",
    // FLEA_Db_Exception_InvalidInsertID
    0x06ff008 => "Can not get the ID generated for an AUTO_INCREMENT column by previous INSERT query.",
    // FLEA_Db_Exception_MissingLink
    0x06ff009 => "Association object \"%s\" does not exist.",

    // FLEA_Db_Exception_InvalidLinkType
    0x0202001 => 'Invalid table association type "%s".',
    // FLEA_Db_Exception_MissingLinkOption
    0x0202002 => 'Option "%s" is required when creating table association.',

    // FLEA_View_Exception_NotConfigurationSmarty
    0x0902001 => 'Application settings "viewConfig[\'smartyDir\']" and constant SMARTY_DIR must be defined for FLEA_View_Smarty.',
    // FLEA_View_Exception_InitSmartyFailed
    0x0902002 => 'Smarty file "%s" is not found.',

    // FLEA_View_Exception_NotConfigurationSmartTemplate
    0x0903001 => 'Application settings "viewConfig[\'smartDir\']" must be defined for FLEA_View_SmartTemplate.',
    // FLEA_View_Exception_InitSmartTemplateFailed
    0x0903002 => 'SmartTemplate file "%s" is not found.',


    // FLEA_View_Exception_NotConfigurationLite
    0x0904001 => 'Application settings "viewConfig[\'liteDir\']" must be defined for FLEA_View_Lite.',
    // FLEA_View_Exception_InitLiteFailed
    0x0904002 => 'TemplateLite file: "%s" not found.',
);
