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
 * 定义一组便于生成表单元控件的方法
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Html.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * 生成一个下拉列表框
 *
 * @param string $name
 * @param array $arr
 * @param mixed $selected
 * @param string $extra
 */
function html_dropdown_list($name, $arr, $selected = null, $extra = null) {
    echo "<select name=\"{$name}\" {$extra} >\n";
    foreach ($arr as $value => $title) {
        echo '<option value="' . h($value) . '"';
        if ($selected == $value) { echo ' selected'; }
        echo '>' . h($title) . "&nbsp;&nbsp;</option>\n";
    }
    echo "</select>\n";
}

/**
 * 生成一组单选框
 *
 * @param string $name
 * @param array $arr
 * @param mixed $checked
 * @param string $separator
 * @param string $extra
 */
function html_radio_group($name, $arr, $checked = null, $separator = '', $extra = null) {
    $ix = 0;
    foreach ($arr as $value => $title) {
        $value_h = h($value);
        $title = t($title);
        echo "<input name=\"{$name}\" type=\"radio\" id=\"{$name}_{$ix}\" value=\"{$value_h}\" ";
        if ($value == $checked) {
            echo "checked=\"checked\"";
        }
        echo " {$extra} />";
        echo "<label for=\"{$name}_{$ix}\">{$title}</label>";
        echo $separator;
        $ix++;
        echo "\n";
    }
}

/**
 * 生成一组多选框
 *
 * @param string $name
 * @param array $arr
 * @param array $selected
 * @param string $separator
 * @param string $extra
 */
function html_checkbox_group($name, $arr, $selected = array(), $separator = '', $extra = null) {
    $ix = 0;
    if (!is_array($selected)) {
        $selected = array($selected);
    }
    foreach ($arr as $value => $title) {
        $value_h = h($value);
        $title = t($title);
        echo "<input name=\"{$name}[]\" type=\"checkbox\" id=\"{$name}_{$ix}\" value=\"{$value_h}\" ";
        if (in_array($value, $selected)) {
            echo "checked=\"checked\"";
        }
        echo " {$extra} />";
        echo "<label for=\"{$name}_{$ix}\">{$title}</label>";
        echo $separator;
        $ix++;
        echo "\n";
    }
}

/**
 * 生成一个多选框
 *
 * @param string $name
 * @param int $value
 * @param boolean $checked
 * @param string $label
 * @param string $extra
 */
function html_checkbox($name, $value = 1, $checked = false, $label = '', $extra = null) {
    echo "<input name=\"{$name}\" type=\"checkbox\" id=\"{$name}_1\" value=\"{$value}\"";
    if ($checked) { echo " checked"; }
    echo " {$extra} />\n";
    if ($label) {
        echo "<label for=\"{$name}_1\">" . h($label) . "</label>\n";
    }
}

/**
 * 生成一个文本输入框
 *
 * @param string $name
 * @param string $value
 * @param int $width
 * @param int $maxLength
 * @param string $extra
 */
function html_textbox($name, $value = '', $width = null, $maxLength = null, $extra = null) {
    echo "<input name=\"{$name}\" type=\"text\" value=\"" . h($value) . "\" ";
    if ($width) {
        echo "size=\"{$width}\" ";
    }
    if ($maxLength) {
        echo "maxlength=\"{$maxLength}\" ";
    }
    echo " {$extra} />\n";
}

/**
 * 生成一个密码输入框
 *
 * @param string $name
 * @param string $value
 * @param int $width
 * @param int $maxLength
 * @param string $extra
 */
function html_password($name, $value = '', $width = null, $maxLength = null, $extra = null) {
    echo "<input name=\"{$name}\" type=\"password\" value=\"" . h($value) . "\" ";
    if ($width) {
        echo "size=\"{$width}\" ";
    }
    if ($maxLength) {
        echo "maxlength=\"{$maxLength}\" ";
    }
    echo " {$extra} />\n";
}

/**
 * 生成一个多行文本输入框
 *
 * @param string $name
 * @param string $value
 * @param int $width
 * @param int $height
 * @param string $extra
 */
function html_textarea($name, $value = '', $width = null, $height = null, $extra = null) {
    echo "<textarea name=\"{$name}\"";
    if ($width) { echo "cols=\"{$width}\" "; }
    if ($height) { echo "rows=\"{$height}\" "; }
    echo " {$extra} >";
    echo h($value);
    echo "</textarea>\n";
}

/**
 * 生成一个隐藏域
 *
 * @param string $name
 * @param string $value
 * @param string $extra
 */
function html_hidden($name, $value = '', $extra = null) {
    echo "<input name=\"{$name}\" type=\"hidden\" value=\"";
    echo h($value);
    echo "\" {$extra} />\n";
}

/**
 * 生成一个文件上传域
 *
 * @param string $name
 * @param int $width
 * @param string $extra
 */
function html_filefield($name, $width = null, $extra = null) {
    echo "<input name=\"{$name}\" type=\"file\"";
    if ($width) {
        echo " size=\"{$width}\"";
    }
    echo " {$extra} />\n";
}

/**
 * 生成 form 标记
 *
 * @param string $name
 * @param string $action
 * @param string $method
 * @param string $onsubmit
 * @param string $extra
 */
function html_form($name, $action, $method='post', $onsubmit='', $extra = null) {
    echo "<form name=\"{$name}\" action=\"{$action}\" method=\"{$method}\" ";
    if ($onsubmit) {
        echo "onsubmit=\"{$onsubmit}\"";
    }
    echo " {$extra} >\n";
}

/**
 * 关闭 form 标记
 */
function html_form_close() {
    echo "</form>\n";
}
