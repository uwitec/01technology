<?php
// 配置文件
/* 取得当前ecshop所在的根目录 */
//define('ROOT_PATH', str_replace('includes/init.php', '', str_replace('\\', '/', __FILE__)));

define('ROOT_PATH', '');
define('FILE_PATH', dirname(__FILE__));
define('APP_DIR', FILE_PATH . DS . 'APP');
define('SM_DIR', FILE_PATH . DS . 'Stuff' . DS . 'Smarty');
define('TEMP_DIR', FILE_PATH . DS . 'templates');
define('TEMPC_DIR', FILE_PATH . DS . 'templates_c');
define('CACHE_DIR', FILE_PATH .DS. 'FLEA' .DS. '_Cache');

define('STUFF_DIR', ROOT_PATH  . '/Stuff');
define('FCK_DIR', STUFF_DIR . '/fckeditor');
define('IMG_DIR', STUFF_DIR . '/images');
define('CSS_DIR', STUFF_DIR . '/css');
define('JS_DIR', STUFF_DIR . '/js');
define('GOODS_DIR', IMG_DIR . '/goods');
define('ADS_DIR', '/ads');


define('MAX_FILES', 1500 * 1500);
define('ALLOW_EXTS', 'jpg,png,gif');
define('UPLOAD_IMG_DIR', FILE_PATH .DS. 'Stuff' .DS. 'images' .DS. 'goods');


// 定义登录后台所需的用户名密码
define('ADMIN_NAME', 'admin');
define('ADMIN_PSW', 'admin');

// 配置文件
$appInf = array(
    /**
     * 是否将 url 参数中包含的控制器名字和动作名字强制转为小写
     */
	'urlLowerChar' => true,
    /**
     * 数据表的全局前缀
     */
    'dbTablePrefix' => 'ws_',
    /**
     * 数据库配置，可以是数组，也可以是 DSN 字符串
     */
	'dbDSN' => array(
        'driver'    => 'mysql',
        'host'      => 'localhost',
        'login'     => 'lnwwyy.com',
        'password'  => 'zaq1xsw2',
        'database'  => 'lnwwyy.com'),
    /**
     * meta设置
     */
	'metas' => array(
		'robots' => 'all',
		'author' => '零壹科技 www.in01.com.cn',
		'copyright' => '辽宁万维医药有限公司',
		'description' => '辽宁万维医药有限公司',
		'keywords' => '思清,柴芩清宁胶囊,脑中卒,辽宁总经销',
		'title' => '辽宁万维医药有限公司',
		'charset' => 'utf-8'
     ),
    /**
     * 要使用的模板引擎，'PHP' 表示使用 PHP 语言本身作模板引擎
     */
    'view' => 'FLEA_View_Smarty',
    /**
     * 模板引擎SMARTY配置
     */
    'viewConfig' => array(
        'smartyDir'         => realpath(SM_DIR),
        'template_dir'      => TEMP_DIR,
        'compile_dir'       => TEMPC_DIR,
        'left_delimiter'    => '{{',
        'right_delimiter'   => '}}'),
    /**
     * FleaPHP 内部及 cache 系列函数使用的缓存目录
     * 应用程序必须设置该选项才能使用 cache 功能。
     */
	'internalCacheDir' => CACHE_DIR,

    /**
     * 指示 FleaPHP 应用程序内部处理数据和输出内容要使用的编码
     */
    'responseCharset' => 'utf-8',

    /**
     * 当 FleaPHP 连接数据库时，用什么编码传递数据
     */
    'databaseCharset' => 'utf8'
);
FLEA::loadAppInf($appInf);
?>