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
 * 定义 FLEA_Language 类
 *
 * 使用界面多语言支持，要做如下工作：
 *
 * <strong>1、 修改应用程序设置</strong>
 * - multiLanguageSupport 指定为 true，启用多语言支持；
 * - languageFilesDir 指定一个目录，表示语言文件所在根目录；
 * - defaultLanguage 指定默认语言名，例如 chinese-utf8、chinese-gb2312 等。
 *
 * <strong>2、 准备语言目录和字典文件</strong>
 *
 * 假设 languageFilesDir 指向 d:\www\myapp\APP\Languages，
 * 那么就要在 d:\www\myapp\APP\Languages 下建立不同语言的子目录，
 * 例如 chinese-utf8、chinese-gb2312。
 *
 * 因此应用程序如果是要载入 chinese-utf8 的字典文件，那么实际的字典文件就存放在
 * d:\www\myapp\APP\Languages\chinese-utf8\ 目录。
 *
 * 字典文件的名字可以随意定，但最好有一定意义。
 * 例如用于用户界面的字典文件可以是 ui.php 或者 UserInterface.php。
 *
 * 字典文件的内容很简单，就是用 return 返回一个数组，例如：
 *
 * <code>
 * <?php
 * // d:\www\myapp\APP\Languages\chinese-utf8\UserInterface.php
 * // 用于 chinese-utf8 语言的用户界面字典文件
 *
 * return array(
 *     'applicationTitle' => '应用程序的标题',
 *     'authorName' => '作者名',
 *     'copyright' => '版权所有',
 * );
 *
 * </code>
 *
 * <strong>3、 在应用程序中使用字典文件</strong>
 *
 * 应用程序用 load_language($dictname, $language = null) 载入指定的语言文件。
 * $dictname 参数指定字典名，$language 参数指定语言名。
 * 假如没有指定 $language 参数，则使用应用程序设置 defaultLanguage 的值作为
 * $language 参数的值。
 *
 * <code>
 * // 载入 chinese-utf8 的指定字典文件
 * load_language('UserInterface', 'chinese-utf8');
 * </code>
 *
 * <strong>4、 获取字典中对应的文本</strong>
 *
 * 载入字典文件后，就可以通过字典文件查询原文和翻译后的文本了。
 * 使用 _T($key, $language = null) 获取指定字符串的翻译文本，例如：
 *
 * <code>
 * echo _T('applicationTitle', 'chinese-utf8');
 * </code>
 *
 * 显示结果将是前面字典文件中定义的'应用程序的标题'。
 *
 * <strong>5、 简化操作</strong>
 *
 * 每次都给 _T() 指定第二个参数显然不方便，所以我们可以在应用程序开始执行时，
 * 先获取用户选择的界面语言，然后用 set_app_inf('defaultLanguage', $language)
 * 设置默认语言。
 *
 * 这样后续的 _T() 就不用指定第二个参数了。
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Language.php 1005 2007-11-03 07:43:55Z qeeyuan $
 */

/**
 * 调用 FLEA_Language::get() 获取翻译
 *
 * 用法：
 * <code>
 * $msg = _T('ENGLISH', 'chinese');
 * $msg = sprintf(_T('ENGLISH: %s'), 'chinese');
 * </code>
 *
 * @param string $key
 * @param string $language 指定为 '' 时表示从默认语言包中获取翻译
 *
 * @return string
 */
function _T($key, $language = '')
{
    static $instance = null;
    if (!isset($instance['obj'])) {
        $instance = array();
        $obj =& FLEA::getSingleton('FLEA_Language');
        $instance = array('obj' => & $obj);
    }
    return $instance['obj']->get($key, $language);
}

/**
 * 载入语言字典文件
 *
 * @param string $dictname
 * @param string $language 指定为 '' 时表示将字典载入默认语言包中
 * @param boolean $noException
 *
 * @return boolean
 */
function load_language($dictname, $language = '', $noException = false)
{
    static $instance = null;
    if (!isset($instance['obj'])) {
        $instance = array();
        $obj =& FLEA::getSingleton('FLEA_Language');
        $instance = array('obj' => & $obj);
    }
    return $instance['obj']->load($dictname, $language, $noException);
}

/**
 * FLEA_Language 提供了语言转换功能
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Language
{
    /**
     * 保存当前载入的字典
     *
     * @var array
     */
    var $_dict = array();

    /**
     * 指示哪些语言文件已经被载入
     *
     * @var array
     */
    var $_loadedFiles = array();

    /**
     * 构造函数
     *
     * @return FLEA_Language
     */
    function FLEA_Language()
    {
        $autoload = FLEA::getAppInf('autoLoadLanguage');
        if (!is_array($autoload)) {
            $autoload = explode(',', $autoload);
        }
        foreach ($autoload as $load) {
            $load = trim($load);
            if ($load != '') {
                $this->load($load);
            }
        }
    }

    /**
     * 载入指定语言的字典文件
     *
     * 所有的语言文件均按照“语言/字典名.php”的形式保存在由应用程序设置
     * 'languageFilesDir' 指定的目录中。默认的保存目录为 FLEA/Languages。
     *
     * 如果没有指定 $language 参数，则载入由应用程序设置 'defaultLanguage'
     * 指定的语言目录下的文件。
     *
     * $language 和 $dicname 参数均只能使用 26 个字母、10 个数字
     * 和 “-”、“_” 符号。并且为全小写。
     *
     * @param string $dictname 字典名，例如 'fleaphp'、'rbac'
     * @param string $language 指定为 '' 时表示将字典载入默认语言包中
     * @param boolena $noException
     */
    function load($dictname, $language = '', $noException = false)
    {
        $dictnames = explode(',', $dictname);
        foreach ($dictnames as $dictname) {
            $dictname = trim($dictname);
            if ($dictname == '') { continue; }

            $dictname = preg_replace('/[^a-z0-9\-_]+/i', '', strtolower($dictname));
            $language = preg_replace('/[^a-z0-9\-_]+/i', '', strtolower($language));
            if ($language == '') {
                $language = FLEA::getAppInf('defaultLanguage');
                $default = true;
            } else {
                $default = false;
            }

            $filename = FLEA::getAppInf('languageFilesDir') . DS .
                $language . DS . $dictname . '.php';
            if (isset($this->_loadedFiles[$filename])) { continue; }

            if (is_readable($filename)) {
                $dict = require($filename);
                $this->_loadedFiles[$filename] = true;
                if (isset($this->_dict[$language])) {
                    $this->_dict[$language] = array_merge($this->_dict[$language], $dict);
                } else {
                    $this->_dict[$language] = $dict;
                }
                if ($default) {
                    $this->_dict[0] =& $this->_dict[$language];
                }
            } else if (!$noException) {
                FLEA::loadClass('FLEA_Exception_ExpectedFile');
                return __THROW(new FLEA_Exception_ExpectedFile($filename));
            }
        }
    }

    /**
     * 返回指定键的对应语言翻译，没有找到翻译时返回键
     *
     * @param string $key
     * @param string $language 指定为 '' 时表示从默认语言包中获取翻译
     *
     * @return string
     */
    function get($key, $language = '')
    {
        if ($language == '') { $language = 0; }
        return isset($this->_dict[$language][$key]) ?
            $this->_dict[$language][$key] :
            $key;
    }
}
