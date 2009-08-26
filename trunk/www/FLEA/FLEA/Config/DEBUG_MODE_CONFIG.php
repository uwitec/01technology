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
 * “调试”模式的 FleaPHP 应用程序的默认设置
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Config
 * @version $Id: DEBUG_MODE_CONFIG.php 1037 2008-04-19 21:19:55Z qeeyuan $
 */

return array(
    // {{{ 核心配置

    /**
     * 应用层序的默认时区设置，仅针对 PHP 5.1 以后版本
     *
     * 如果该设置为 null，则以服务器设置为准。如果服务器没有设置时区，则设置为 Asia/ShangHai。
     */
    'defaultTimezone'           => null,

    /**
     * 指示使用 MVC 模式时，需要载入哪一个文件
     */
    'MVCPackageFilename'        => FLEA_DIR . '/Controller/Action.php',

    /**
     * 指示控制器的 url 参数名和默认控制器名
     *
     * 控制器名字只能是a-z字母和0-9数字，以及“_”下划线。
     */
    'controllerAccessor'        => 'controller',
    'defaultController'         => 'Default',

    /**
     * 指示 动作方法的 url 参数名和默认 动作方法名
     */
    'actionAccessor'            => 'action',
    'defaultAction'             => 'index',

    /**
     * url 参数的传递模式，可以是标准、PATHINFO、URL 重写等模式
     */
    'urlMode'                   => URL_STANDARD,

    /**
     * 指示默认的应用程序入口文件名
     */
    'urlBootstrap'              => 'index.php',

    /**
     * 指示在生成 url 时，是否总是使用应用程序入口文件名，仅限 URL_STANDARD 模式
     *
     * 如果该设置为 false，则生成的 url 类似：
     *
     * http://www.example.com/?controller=xxx&action=yyy
     */
    'urlAlwaysUseBootstrap'     => true,

    /**
     * 指示在生成 url 时，是否总是使用完整的控制器名和动作名
     *
     * 如果该设置为 false，则默认的控制器和动作名不会出现在 url 中
     */
    'urlAlwaysUseAccessor'      => true,

    /**
     * 指示在 PATHINFO 和 REWRITE 模式下，用什么符号作为 URL 参数名和参数值的连接字符
     */
    'urlParameterPairStyle'     => '/',

    /**
     * 是否将 url 参数中包含的控制器名字和动作名字强制转为小写
     */
    'urlLowerChar'              => false,

    /**
     * 调用 url() 函数时，要调用的 callback 方法
     */
    'urlCallback'               => null,

    /**
     * 控制器类名称前缀
     */
    'controllerClassPrefix'     => 'Controller_',

    /**
     * 控制器中，动作方法名的前缀和后缀
     * 使用前缀和后缀可以进一步保护控制器中的私有方法
     */
    'actionMethodPrefix'        => 'action',
    'actionMethodSuffix'        => '',

    /**
     * 应用程序要使用的 url 调度器
     */
    'dispatcher'                => 'FLEA_Dispatcher_Simple',

    /**
     * 调度器调度失败（例如控制器或控制器方法不存在）后，要调用的处理程序
     */
    'dispatcherFailedCallback'  => null,

    /**
     * FleaPHP 内部及 cache 系列函数使用的缓存目录
     * 应用程序必须设置该选项才能使用 cache 功能。
     */
    'internalCacheDir'          => null,

    /**
     * 指示要自动载入的文件
     */
    'autoLoad'                  => array(),

    /**
     * 指示是否载入 session 提供程序
     */
    'sessionProvider'           => null,

    /**
     * 指示是否自动起用 session 支持
     */
    'autoSessionStart'          => true,

    /**
     * 指示使用哪些过滤器对 HTTP 请求进行过滤
     */
    'requestFilters'            => array(),

    // }}}

    // {{{ 数据库相关

    /**
     * 数据库配置，可以是数组，也可以是 DSN 字符串
     */
    'dbDSN'                     => null,

    /**
     * 指示构造 TableDataGateway 对象时，是否自动连接到数据库
     */
    'dbTDGAutoInit'             => true,

    /**
     * 数据表的全局前缀
     */
    'dbTablePrefix'             => '',

    /**
     * 数据表元数据缓存时间（秒），如果 dbMetaCached 设置为 false，则不会缓存数据表元数据
     * 通常开发时，该设置为 10，以便修改数据库表结构后应用程序能够立刻刷新元数据
     */
    'dbMetaLifetime'            => 10,

    /**
     * 指示是否缓存数据表的元数据
     */
    'dbMetaCached'              => true,

    // }}}

    // {{{ View 相关

    /**
     * 要使用的模板引擎，'PHP' 表示使用 PHP 语言本身作模板引擎
     */
    'view'                      => 'PHP',

    /**
     * 模板引擎要使用的配置信息
     */
    'viewConfig'                => null,

    /**
     * 初始化 Ajax 时要载入的类
     */
    'ajaxClassName'             => 'FLEA_Ajax',

    /**
     * 初始化 WebControls 时要载入的类
     */
    'webControlsClassName'      => 'FLEA_WebControls',

    /**
     * WebControls 扩展控件的保存目录
     */
    'webControlsExtendsDir'     => null,

    // }}}

    // {{{ I18N

    /**
     * 指示 FleaPHP 应用程序内部处理数据和输出内容要使用的编码
     */
    'responseCharset'           => 'utf-8',

    /**
     * 当 FleaPHP 连接数据库时，用什么编码传递数据
     */
    'databaseCharset'           => 'utf8',

    /**
     * 是否自动输出 Content-Type: text/html; charset=responseCharset
     */
    'autoResponseHeader'        => true,

    /**
     * 指示是否启用多语言支持
     */
    'multiLanguageSupport'      => false,

    /**
     * 指定提供多语言支持的提供程序
     */
    'languageSupportProvider'   => 'FLEA_Language',

    /**
     * 指示语言文件的保存位置
     */
    'languageFilesDir'          => null,

    /**
     * 指示默认语言
     */
    'defaultLanguage'           => 'chinese-utf8',

    /**
     * 自动载入的语言文件
     */
    'autoLoadLanguage'          => null,

    // }}}

    // {{{ FLEA_Dispatcher_Auth 和 RBAC 组件

    /**
     * 调度器要使用的验证服务提供程序
     */
    'dispatcherAuthProvider'    => 'FLEA_Rbac',

    /**
     * 指示 RBAC 组件要使用的默认 ACT 文件
     */
    'defaultControllerACTFile'  => '',

    /**
     * 指示 RBAC 组件是否在没有找到控制器的 ACT 文件时，
     * 是否从默认 ACT 文件中查询控制器的 ACT
     */
    'autoQueryDefaultACTFile'   => false,

    /**
     * 当控制器没有提供 ACT 文件时，显示警告信息
     */
    'controllerACTLoadWarning'  => true,

    /**
     * 指示当没有为控制器提供 ACT 时，要使用的默认 ACT
     */
    'defaultControllerACT'      => null,

    /**
     * 全局 ACT，当没有指定 ACT 时则从全局 ACT 中查找指定控制器的 ACT
     */
    'globalACT'                 => null,

    /**
     * 用户没有权限访问控制器或控制器方法时，要调用的处理程序
     */
    'dispatcherAuthFailedCallback' => null,

    /**
     * 指示 RBAC 组件用什么键名在 session 中保存用户数据
     *
     * 如果在一个域名下同时运行多个应用程序，
     * 请务必为每一个应用程序使用自己独一无二的键名
     */
    'RBACSessionKey'            => 'RBAC_USERDATA',

    // }}}

    // {{{ 日志和错误处理
    /**
     * 指示是否启用日志服务
     */
    'logEnabled'                => true,

    /**
     * 指示日志服务的程序
     */
    'logProvider'               => 'FLEA_Log',

    /**
     * 指示用什么目录保存日志文件
     *
     * 如果没有指定日志存放目录，则保存到内部缓存目录中
     */
    'logFileDir'                => null,

    /**
     * 指示用什么文件名保存日志
     */
    'logFilename'               => 'access.log',

    /**
     * 指示当日志文件超过多少 KB 时，自动创建新的日志文件，单位是 KB，不能小于 512KB
     */
    'logFileMaxSize'            => 4096,

    /**
     * 指示哪些级别的错误要保存到日志中
     */
    'logErrorLevel'             => 'notice, debug, warning, error, exception, log',

    /**
     * 异常处理例程
     */
    'exceptionHandler'          => '__FLEA_EXCEPTION_HANDLER',

    /**
     * 指示是否显示错误信息
     */
    'displayErrors'             => true,

    /**
     * 指示是否显示友好的错误信息
     */
    'friendlyErrorsMessage'     => true,

    /**
     * 指示是否在错误信息中显示出错位置的源代码
     */
    'displaySource'             => true,

    // }}}

    // {{{ 助手库

    /**
     * 数据验证服务助手
     */
    'helper.verifier'           => 'FLEA_Helper_Verifier',

    /**
     * 加密算法助手
     */
    'helper.encryption'         => 'FLEA_Helper_Encryption',

    /**
     * 数组处理助手
     */
    'helper.array'              => 'FLEA_Helper_Array',

    /**
     * 文件系统操作助手
     */
    'helper.file'               => 'FLEA_Helper_FileSystem',

    /**
     * 图像处理助手
     */
    'helper.image'              => 'FLEA_Helper_Image',

    /**
     * 分页助手
     */
    'helper.pager'              => 'FLEA_Helper_Pager',

    /**
     * 文件上传助手
     */
    'helper.uploader'           => 'FLEA_Helper_FileUploader',

    /**
     * YAML 助手
     */
    'helper.yaml'               => 'FLEA_Helper_Yaml',

    /**
     * HTML 助手
     */
    'helper.html'               => 'FLEA_Helper_Html',

// }}}

    // {{{ FLEA_Session_Db 设置

    /**
     * 指示使用应用程序中哪一个 DSN 连接 session 数据表
     */
    'sessionDbDSN'              => 'dbDSN',

    /**
     * 指示保存 session 的数据表名称
     */
    'sessionDbTableName'        => 'sessions',

    /**
     * 指示保存 session id 的字段名
     */
    'sessionDbFieldId'          => 'sess_id',

    /**
     * 指示保存 session 数据的字段名
     */
    'sessionDbFieldData'        => 'sess_data',

    /**
     * 指示保存 session 最后活动时间的字段名
     */
    'sessionDbFieldActivity'   => 'activity',

    /**
     * 指示 session 的有效期
     *
     * 0 表示由 PHP 运行环境决定，其他数值为超过最后一次活动时间多少秒后失效
     */
    'sessionDbLifeTime'         => 1440,

    // }}}
);
