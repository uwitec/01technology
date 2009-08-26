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
 * 定义 FLEA_Helper_Verifier 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Verifier.php 1018 2007-12-04 23:41:47Z qeeyuan $
 */

/**
 * FLEA_Helper_Verifier 对象根据一系列验证规则对指定的数据进行验证
 *
 * 验证规则由多个规则组成，每个规则用于验证一个字段。
 *
 * 每个规则可以包含下列基本属性：
 * name:            字段名
 * type:            字段类型
 * simpleType:      简单字段类型
 * maxLength:       最大长度
 * notNull:         是否不允许保存 NULL 值
 * notEmpty:        是否不允许为空字符串
 * binary:          是否是二进制数据
 * unsigned:        是否是无符号数值
 * hasDefault:      是否有默认值
 * defaultValue:    默认值
 *
 * 如果 notNull 为 true，而 hasDefault 为 false，则表示该字段必须有内容。
 *
 * simpleType 属性可以有下列值：
 *  C        - 长度小于等于 250 的字符串
 *  X        - 长度大于 250 的字符串
 *  B        - 二进制数据
 *  N        - 数值或者浮点数
 *  D        - 日期
 *  T        - TimeStamp
 *  L        - 逻辑布尔值
 *  I        - 整数
 *  R        - 自动增量或计数器
 *
 * 基本属性和利用 SDBO::metaColumns() 方法取得的字段信息完全一致。
 * 因此可以直接将 metaColumns() 的返回结果作为验证规则。
 *
 * 为了获得更强的验证能力，还可以使用下列扩展属性：
 *
 * complexType:     复杂字段类型
 * min:             最小值（仅用于数值型字段）
 * max:             最大值（仅用于数值型字段）
 * minLength:       最小长度（仅用于字符型和文本型字段）
 * maxLength:       最大长度（仅用于字符行和文本型字段）
 *
 * 对于 complexType 属性，可以有下列值：
 * NUMBER     - 数值（整数、浮点数）
 * INT        - 整数
 * ASCII      - ASCII 字符串（所有编码小于等于 127 的字符）
 * EMAIL      - Email 地址
 * DATE       - 日期（所有 GNU Date Input Formats，例如 yyyy/mm/dd、yyyy-mm-dd）
 * TIME       - 时间（所有 GNU Date Input Formats，例如 hh:mm:ss）
 * IPv4       - IPv4 地址（格式为 a.b.c.h）
 * OCTAL      - 八进制数值
 * BINARY     - 二进制数值
 * HEX        - 十六进制数值
 * DOMAIN     - Internet 域名
 * ANY        - 任意类型
 * STRING     - 字符串（等同于任意类型）
 * ALPHANUM   - 文字和数字（26个字母和0－9）
 * ALPHA      - 文字（26个字母）
 * ALPHANUMX  - 26个字母、10个数字以及 _ 符号
 * ALPHAX     - 26个字母以及 - 符号
 *
 * 在客户端可以通过 verifier.js 提供的验证服务对数据进行验证，
 * 但客户端验证仅提供有限的验证能力。包括：
 *
 * notNull, hasDefault, min, max, minLength, maxLength
 *
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Helper_Verifier
{
    /**
     * 检查所有字段，返回验证结果
     *
     * @param array $data
     * @param array $rules
     * @param mixed $skip
     *
     * @return array
     */
    function checkAll(& $data, & $rules, $skip = 0)
    {
        $result = array();
        foreach ($rules as $rule) {
            $name = $rule['name'];
            if ($skip === 1 || $skip === true || $skip === 'empty') {
                if (!isset($data[$name]) || empty($data[$name])) { continue; }
            } elseif ($skip === 2 || $skip === 'noset') {
                if (!isset($data[$name])) { continue; }
            }

            do {
                // 如果 notNull 为 true，而 hasDefault 为 false，则表示该字段必须有内容
                if (isset($rule['notNull'])) {
                    if ($rule['notNull']) {
                        if (isset($rule['hasDefault']) && $rule['hasDefault']) { break; }
                        if (isset($rule['simpleType']) && $rule['simpleType'] == 'R') {
                            break;
                        }
                        if (!isset($data[$name]) || is_null($data[$name])) {
                            $result[$name] = array('check' => 'notNull', 'rule' => $rule);
                            break;
                        }
                    } else {
                        if (!isset($data[$name]) || is_null($data[$name]) || $data[$name] === '') { break; }
                    }
                }
                if (isset($rule['notEmpty'])) {
                    if ($rule['notEmpty']) {
                        if (isset($rule['hasDefault']) && $rule['hasDefault']) { break; }
                        if (isset($rule['simpleType']) && $rule['simpleType'] == 'R') {
                            break;
                        }
                        if (!isset($data[$name]) || $data[$name] == '') {
                            $result[$name] = array('check' => 'notEmpty', 'rule' => $rule);
                            break;
                        }
                    } else {
                        if (!isset($data[$name]) || $data[$name] == '') {
                            break;
                        }
                    }
                }

                $ret = $this->check($data[$name], $rule);
                if ($ret !== true) {
                    $result[$name] = array('check' => $ret, 'rule' => $rule);
                }
            } while (false);
        }
        return $result;
    }

    /**
     * 用指定规则验证值，验证通过返回 ture，否则返回没有通过的验证规则名
     *
     * @param mixed $value
     * @param array $rule
     *
     * @return boolean
     */
    function check($value, & $rule)
    {
        // 首先使用 simpleType 验证值（如果 simpleType 属性存在）
        $checkLength = false;
        $checkMinMax = false;
        $ret = 'simpleType';
        if (isset($rule['simpleType'])) {
            switch ($rule['simpleType']) {
            case 'C': // 长度小于等于 250 的字符串
                if (strlen($value) > 250) { return $ret; }
                $checkLength = true;
                break;
            case 'N': // 数值或者浮点数
                if (!is_numeric($value)) { return $ret; }
                $checkMinMax = true;
                break;
            case 'D': // 日期
                $test = @strtotime($value);
                if ($test === false || $test === -1) { return $ret; }
                break;
            case 'I': // 整数
                if (!is_numeric($value)) { return $ret; }
                if (intval($value) != $value) { return $ret; }
                $checkMinMax = true;
                break;
            case 'X': // 长度大于 250 的字符串
            case 'B': // 二进制数据
                $checkLength = true;
                break;
            case 'T': // TimeStamp
            case 'L': // 逻辑布尔值
                break;
            case 'R': // 自动增量或计数器
                $checkMinMax = true;
                break;
            default:
            }
        } else {
            $checkLength = true;
            $checkMinMax = true;
        }

        // 接着使用 complexType 验证值（如果 complexType 属性存在）
        $ret = 'complexType';
        if (isset($rule['complexType'])) {
            $func = 'is' . $rule['complexType'];
            if (!method_exists($this, $func)) {
                FLEA::loadClass('FLEA_Exception_InvalidArguments');
                __THROW(new FLEA_Exception_InvalidArguments('$rule[\'complexType\']',
                        $rule['complexType']));
                return null;
            }
            if (!$this->{$func}($value)) { return $ret; }
        }

        // min/max/minLength/maxLength 验证
        if ($checkMinMax) {
            $ret = 'min';
            if (isset($rule['min']) && $value < $rule['min']) { return $ret; }
            $ret = 'max';
            if (isset($rule['max']) && $value > $rule['max']) { return $ret; }
        }
        $ret = 'length';
        if ($checkLength) {
            $ret = 'minLength';
            if (isset($rule['minLength']) && $rule['minLength'] > 0 &&
                strlen($value) < $rule['minLength']) {
                return $ret;
            }
            $ret = 'maxLength';
            if (isset($rule['maxLength']) && $rule['maxLength'] > 0 &&
                strlen($value) > $rule['maxLength']) {
                return $ret;
            }
        }
        $ret = null;

        return true;
    }

    /**
     * 数字
     */
    function isNUMBER($value)
    {
        return is_numeric($value);
    }

    /**
     * 整数
     */
    function isINT($value)
    {
        return strlen(intval($value)) == strlen($value) && is_numeric($value);
    }

    /**
     * ASCII 字符串（所有编码小于等于 127 的字符）
     */
    function isASCII($value)
    {
        $ar = array();
        $count = preg_match_all('/[\x20-\x7f]/', $value, $ar);
        return $count == strlen($value);
    }

    /**
     * Email 地址
     */
    function isEMAIL($value)
    {
        return preg_match('/^[A-Za-z0-9]+([._\-\+]*[A-Za-z0-9]+)*@([A-Za-z0-9]+[-A-Za-z0-9]*[A-Za-z0-9]+\.)+[A-Za-z0-9]+$/', $value) != 0;
    }

    /**
     * 日期（所有 GNU Date Input Formats，例如 yyyy/mm/dd、yyyy-mm-dd）
     */
    function isDATE($value)
    {
        $test = @strtotime($value);
        return $test !== -1 && $test !== false;
    }

    /**
     * 时间（所有 GNU Date Input Formats，例如 hh:mm:ss）
     */
    function isTIME($value)
    {
        $test = strtotime($value);
        return $test !== -1 && $test !== false;
    }

    /**
     * IPv4 地址（格式为 a.b.c.h）
     */
    function isIPv4($value)
    {
        $test = ip2long($value);
        return $test !== -1 && $test !== false;
    }

    /**
     * 八进制数值
     */
    function isOCTAL($value)
    {
        return preg_match('/0[0-7]+/', $value) != 0;
    }

    /**
     * 二进制数值
     */
    function isBINARY($value)
    {
        return preg_match('/[01]+/', $value) != 0;
    }

    /**
     * 十六进制数值
     */
    function isHEX($value)
    {
        return preg_match('/[0-9a-f]+/i', $value) != 0;
    }

    /**
     * Internet 域名
     */
    function isDOMAIN($value)
    {
        return preg_match('/[a-z0-9\.]+/i', $value) != 0;
    }

    /**
     * 任意类型
     */
    function isANY()
    {
        return true;
    }

    /**
     * 字符串（等同于任意类型）
     */
    function isSTRING()
    {
        return true;
    }

    /**
     * 文字和数字（26个字母和0－9）
     */
    function isALPHANUM($value)
    {
        return ctype_alnum($value);
    }

    /**
     * 文字（26个字母）
     */
    function isALPHA($value)
    {
        return ctype_alpha($value);
    }

    /**
     * 26个字母及10个数字
     */
    function isALPHANUMX($value)
    {
        return preg_match('/[^a-z0-9_]/i', $value) == 0;
    }

    /**
     * 26个字母及 - 符号
     */
    function isALPHAX($value)
    {
        return preg_match('/[^a-z\-]/i', $value) == 0;
    }
}
