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
 * FleaPHP 中用于显示异常信息的公共文件
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Error
 * @version $Id: header.php 1005 2007-11-03 07:43:55Z qeeyuan $
 */

if (!defined('RESPONSE_CHARSET')) {
    define('RESPONSE_CHARSET', 'utf-8');
}

/**
 * 显示异常信息及调用堆栈
 *
 * @param FLEA_Exception $ex
 */
function __error_dump_trace($ex)
{
    echo '<strong>Exception: </strong>' . get_class($ex) . "<br />\n";
    if ($ex->getMessage() != '') {
        echo '<strong>Message: </strong>' . $ex->getMessage() . "<br />\n";
    }
    echo "<br />\n";

    if (!FLEA::getAppInf('displaySource')) {
        return;
    }

    $trace = $ex->getTrace();
    $ix = count($trace);
    foreach ($trace as $point) {
        $file = isset($point['file']) ? $point['file'] : null;
        $line = isset($point['line']) ? $point['line'] : null;
        $id = md5("{$file}({$line})");
        $function = isset($point['class']) ? "{$point['class']}::{$point['function']}" : $point['function'];

        $args = array();
        if (is_array($point['args']) && count($point['args']) > 0) {
            foreach ($point['args'] as $arg) {
                switch (gettype($arg)) {
                case 'array':
                    $args[] = 'array(' . count($arg) . ')';
                    break;
                case 'resource':
                    $args[] = gettype($arg);
                    break;
                case 'object':
                    $args[] = get_class($arg);
                    break;
                case 'string':
                    if (strlen($arg) > 30) {
                        $arg = substr($arg, 0, 27) . ' ...';
                    }
                    $args[] = "'{$arg}'";
                    break;
                default:
                    $args[] = $arg;
                }
            }
        }
        $args = implode(", ", $args);

        echo <<<EOT
<hr />
<strong>Filename:</strong> <a href="javascript:switch_filedesc('{$id}');">{$file} [{$line}]</a><br />
#{$ix} {$function}($args)
<div id="{$id}" class="filedesc" style="display: none;">
ARGS:
EOT;

        dump($point['args']);
        echo "SOURCE CODE: <br />\n";
        echo __error_show_source($file, $line);
        echo "\n</div>\n";
        echo "<br />\n";
        $ix--;
    }
}

/**
 * 显示文件源代码
 *
 * @param string $file
 * @param int $line
 * @param int $prev
 * @param int $next
 *
 * @return string
 */
function __error_show_source($file, $line, $prev = 10, $next = 10)
{
    if (!(file_exists($file) && is_file($file))) {
        return '';
    }

    $data = file($file);
    $count = count($data) - 1;

    //count which lines to display
    $start = $line - $prev;
    if ($start < 1) {
        $start = 1;
    }
    $end = $line + $next;
    if ($end > $count) {
        $end = $count + 1;
    }

    //displaying
    $out = '<table cellspacing="0" cellpadding="0">';

    for ($x = $start; $x <= $end; $x++) {
        $out .= "  <tr>\n";
        if ($line != $x) {
            $out .= "    <td class=\"line-num\">";
        } else {
            $out .= "    <td class=\"line-num-break\">";
        }
        $out .= str_repeat('&nbsp;', (strlen($end) - strlen($x)) + 1);
        $out .= $x;
        $out .= "&nbsp;</td>\n";

        $out .= "    <td class=\"source\">&nbsp;";
        $out .= t($data[$x - 1]);
        $out .= "</td>\n  </tr>\n";
    }
    $out .= "</table>\n";
    return $out;
}

/**
 * 显示指定文件的连接
 *
 * @param string $filename
 */
function __error_filelink($filename)
{
    $path = realpath($filename);
    if ($path) {
        echo $path;
    } else {
        echo $filename;
    }
}

/**
 * 输出按照模版要求格式化以后的代码
 *
 * @param string $code
 */
function __error_highlight_string($code)
{
    $text = t2js($code);
    $code = str_replace("<br />", "\n", highlight_string($code, true));

    echo <<<EOT
<pre>
[<a href="#" onclick="copytextToClipboard('{$text}'); return false;">Copy To Clipboard</a>]

{$code}
</pre>
EOT;
}

?>
<meta http-equiv="Content-Type" content="text/html; charset=<?php echo RESPONSE_CHARSET; ?>" />
<style>
<?php readfile(dirname(__FILE__) . '/style.css'); ?>
</style>
<script language="javascript" type="text/javascript">
function switch_filedesc(id)
{
    var el = document.getElementById(id);
    if (el.style.display == 'none') {
        el.style.display = 'block';
    } else {
        el.style.display = 'none';
    }
}

function copytextToClipboard(txt)
{
    if (window.clipboardData) {
        window.clipboardData.clearData();
        window.clipboardData.setData('Text', txt);
    }
}

</script>
