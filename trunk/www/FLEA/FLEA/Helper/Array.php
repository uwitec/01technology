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
 * 定义了一系列用于简化数组操作的函数
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Array.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * 从数组中删除空白的元素（包括只有空白字符的元素）
 *
 * @param array $arr
 * @param boolean $trim
 */
function array_remove_empty(& $arr, $trim = true)
{
    foreach ($arr as $key => $value) {
        if (is_array($value)) {
            array_remove_empty($arr[$key]);
        } else {
            $value = trim($value);
            if ($value == '') {
                unset($arr[$key]);
            } elseif ($trim) {
                $arr[$key] = $value;
            }
        }
    }
}

/**
 * 从一个二维数组中返回指定键的所有值
 *
 * @param array $arr
 * @param string $col
 *
 * @return array
 */
function array_col_values(& $arr, $col)
{
    $ret = array();
    foreach ($arr as $row) {
        if (isset($row[$col])) { $ret[] = $row[$col]; }
    }
    return $ret;
}

/**
 * 将一个二维数组转换为 hashmap
 *
 * 如果省略 $valueField 参数，则转换结果每一项为包含该项所有数据的数组。
 *
 * @param array $arr
 * @param string $keyField
 * @param string $valueField
 *
 * @return array
 */
function array_to_hashmap(& $arr, $keyField, $valueField = null)
{
    $ret = array();
    if ($valueField) {
        foreach ($arr as $row) {
            $ret[$row[$keyField]] = $row[$valueField];
        }
    } else {
        foreach ($arr as $row) {
            $ret[$row[$keyField]] = $row;
        }
    }
    return $ret;
}

/**
 * 将一个二维数组按照指定字段的值分组
 *
 * @param array $arr
 * @param string $keyField
 *
 * @return array
 */
function array_group_by(& $arr, $keyField)
{
    $ret = array();
    foreach ($arr as $row) {
        $key = $row[$keyField];
        $ret[$key][] = $row;
    }
    return $ret;
}

/**
 * 将一个平面的二维数组按照指定的字段转换为树状结构
 *
 * 当 $returnReferences 参数为 true 时，返回结果的 tree 字段为树，refs 字段则为节点引用。
 * 利用返回的节点引用，可以很方便的获取包含以任意节点为根的子树。
 *
 * @param array $arr 原始数据
 * @param string $fid 节点ID字段名
 * @param string $fparent 节点父ID字段名
 * @param string $fchildrens 保存子节点的字段名
 * @param boolean $returnReferences 是否在返回结果中包含节点引用
 *
 * return array
 */
function array_to_tree($arr, $fid, $fparent = 'parent_id',
    $fchildrens = 'childrens', $returnReferences = false)
{
    $pkvRefs = array();
    foreach ($arr as $offset => $row) {
        $pkvRefs[$row[$fid]] =& $arr[$offset];
    }

    $tree = array();
    foreach ($arr as $offset => $row) {
        $parentId = $row[$fparent];
        if ($parentId) {
            if (!isset($pkvRefs[$parentId])) { continue; }
            $parent =& $pkvRefs[$parentId];
            $parent[$fchildrens][] =& $arr[$offset];
        } else {
            $tree[] =& $arr[$offset];
        }
    }
    if ($returnReferences) {
        return array('tree' => $tree, 'refs' => $pkvRefs);
    } else {
        return $tree;
    }
}

/**
 * 将树转换为平面的数组
 *
 * @param array $node
 * @param string $fchildrens
 *
 * @return array
 */
function tree_to_array(& $node, $fchildrens = 'childrens')
{
    $ret = array();
    if (isset($node[$fchildrens]) && is_array($node[$fchildrens])) {
        foreach ($node[$fchildrens] as $child) {
            $ret = array_merge($ret, tree_to_array($child, $fchildrens));
        }
        unset($node[$fchildrens]);
        $ret[] = $node;
    } else {
        $ret[] = $node;
    }
    return $ret;
}

/**
 * 根据指定的键值对数组排序
 *
 * @param array $array 要排序的数组
 * @param string $keyname 键值名称
 * @param int $sortDirection 排序方向
 *
 * @return array
 */
function array_column_sort($array, $keyname, $sortDirection = SORT_ASC)
{
    return array_sortby_multifields($array, array($keyname => $sortDirection));
}

/**
 * 将一个二维数组按照指定列进行排序，类似 SQL 语句中的 ORDER BY
 *
 * @param array $rowset
 * @param array $args
 */
function array_sortby_multifields($rowset, $args)
{
    $sortArray = array();
    $sortRule = '';
    foreach ($args as $sortField => $sortDir) {
        foreach ($rowset as $offset => $row) {
            $sortArray[$sortField][$offset] = $row[$sortField];
        }
        $sortRule .= '$sortArray[\'' . $sortField . '\'], ' . $sortDir . ', ';
    }
    if (empty($sortArray) || empty($sortRule)) { return $rowset; }
    eval('array_multisort(' . $sortRule . '$rowset);');
    return $rowset;
}
