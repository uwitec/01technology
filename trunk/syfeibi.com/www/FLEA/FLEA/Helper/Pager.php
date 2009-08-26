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
 * 定义 FLEA_Helper_Pager 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: Pager.php 1016 2007-11-21 13:53:59Z qeeyuan $
 */

/**
 * FLEA_Helper_Pager 类提供数据查询分页功能
 *
 * FLEA_Helper_Pager 使用很简单，只需要构造时传入 FLEA_Db_TableDataGateway 实例以及查询条件即可。
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Helper_Pager
{
    /**
     * 如果 $this->source 是一个 FLEA_Db_TableDataGateway 对象，则调用
     * $this->source->findAll() 来获取记录集。
     *
     * 否则通过 $this->dbo->selectLimit() 来获取记录集。
     *
     * @var FLEA_Db_TableDataGateway|string
     */
    var $source;

    /**
     * 数据库访问对象，当 $this->source 参数为 SQL 语句时，必须调用
     * $this->setDBO() 设置查询时要使用的数据库访问对象。
     *
     * @var SDBO
     */
    var $dbo;

    /**
     * 查询条件
     *
     * @var mixed
     */
    var $_conditions;

    /**
     * 排序
     *
     * @var string
     */
    var $_sortby;

    /**
     * 计算实际页码时的基数
     *
     * @var int
     */
    var $_basePageIndex = 0;

    /**
     * 每页记录数
     *
     * @var int
     */
    var $pageSize = -1;

    /**
     * 数据表中符合查询条件的记录总数
     *
     * @var int
     */
    var $totalCount = -1;

    /**
     * 数据表中符合查询条件的记录总数
     *
     * @var int
     */
    var $count = -1;

    /**
     * 符合条件的记录页数
     *
     * @var int
     */
    var $pageCount = -1;

    /**
     * 第一页的索引，从 0 开始
     *
     * @var int
     */
    var $firstPage = -1;

    /**
     * 第一页的页码
     *
     * @var int
     */
    var $firstPageNumber = -1;

    /**
     * 最后一页的索引，从 0 开始
     *
     * @var int
     */
    var $lastPage = -1;

    /**
     * 最后一页的页码
     *
     * @var int
     */
    var $lastPageNumber = -1;

    /**
     * 上一页的索引
     *
     * @var int
     */
    var $prevPage = -1;

    /**
     * 上一页的页码
     *
     * @var int
     */
    var $prevPageNumber = -1;

    /**
     * 下一页的索引
     *
     * @var int
     */
    var $nextPage = -1;

    /**
     * 下一页的页码
     *
     * @var int
     */
    var $nextPageNumber = -1;

    /**
     * 当前页的索引
     *
     * @var int
     */
    var $currentPage = -1;

    /**
     * 构造函数中提供的当前页索引，用于 setBasePageIndex() 后重新计算页码
     *
     * @var int
     */
    var $_currentPage = -1;

    /**
     * 当前页的页码
     *
     * @var int
     */
    var $currentPageNumber = -1;

    /**
     * 构造函数
     *
     * 如果 $source 参数是一个 TableDataGateway 对象，则 FLEA_Helper_Pager 会调用
     * 该 TDG 对象的 findCount() 和 findAll() 来确定记录总数并返回记录集。
     *
     * 如果 $source 参数是一个字符串，则假定为 SQL 语句。这时，FLEA_Helper_Pager
     * 不会自动调用计算各项分页参数。必须通过 setCount() 方法来设置作为分页计算
     * 基础的记录总数。
     *
     * 同时，如果 $source 参数为一个字符串，则不需要 $conditions 和 $sortby 参数。
     * 而且可以通过 setDBO() 方法设置要使用的数据库访问对象。否则 FLEA_Helper_Pager
     * 将尝试获取一个默认的数据库访问对象。
     *
     * @param TableDataGateway|string $source
     * @param int $currentPage
     * @param int $pageSize
     * @param mixed $conditions
     * @param string $sortby
     * @param int $basePageIndex
     *
     * @return FLEA_Helper_Pager
     */
    function FLEA_Helper_Pager(& $source, $currentPage, $pageSize = 20, $conditions = null, $sortby = null, $basePageIndex = 0)
    {
        $this->_basePageIndex = $basePageIndex;
        $this->_currentPage = $this->currentPage = $currentPage;
        $this->pageSize = $pageSize;

        if (is_object($source)) {
            $this->source =& $source;
            $this->_conditions = $conditions;
            $this->_sortby = $sortby;
            $this->totalCount = $this->count = (int)$this->source->findCount($conditions);
            $this->computingPage();
        } elseif (!empty($source)) {
            $this->source = $source;
            $sql = "SELECT COUNT(*) FROM ( $source ) as _count_table";
            $this->dbo =& FLEA::getDBO();
            $this->totalCount = $this->count = (int)$this->dbo->getOne($sql);
            $this->computingPage();
        }
    }

    /**
     * 设置分页索引第一页的基数
     *
     * @param int $index
     */
    function setBasePageIndex($index)
    {
        $this->_basePageIndex = $index;
        $this->currentPage = $this->_currentPage;
        $this->computingPage();
    }

    /**
     * 设置当前页码，以便用 findAll() 获得其他页的数据
     *
     * @param int $page
     */
    function setPage($page)
    {
        $this->_currentPage = $page;
        $this->currentPage = $page;
        $this->computingPage();
    }

    /**
     * 设置记录总数，从而更新分页参数
     *
     * @param int $count
     */
    function setCount($count)
    {
        $this->count = $count;
        $this->computingPage();
    }

    /**
     * 设置数据库访问对象
     *
     * @param SDBO $dbo
     */
    function setDBO(& $dbo)
    {
        $this->dbo =& $dbo;
    }

    /**
     * 返回当前页对应的记录集
     *
     * @param string $fields
     * @param boolean $queryLinks
     *
     * @return array
     */
    function & findAll($fields = '*', $queryLinks = true)
    {
        if ($this->count == -1) {
            $this->count = 20;
        }

        $offset = ($this->currentPage - $this->_basePageIndex) * $this->pageSize;
        if (is_object($this->source)) {
            $limit = array($this->pageSize, $offset);
            $rowset = $this->source->findAll($this->_conditions, $this->_sortby, $limit, $fields, $queryLinks);
        } else {
            if (is_null($this->dbo)) {
                $this->dbo =& FLEA::getDBO(false);
            }
            $rs = $this->dbo->selectLimit($this->source, $this->pageSize, $offset);
            $rowset = $this->dbo->getAll($rs);
        }
        return $rowset;
    }

    /**
     * 返回分页信息，方便在模版中使用
     *
     * @param boolean $returnPageNumbers
     *
     * @return array
     */
    function getPagerData($returnPageNumbers = true)
    {
        $data = array(
            'pageSize' => $this->pageSize,
            'totalCount' => $this->totalCount,
            'count' => $this->count,
            'pageCount' => $this->pageCount,
            'firstPage' => $this->firstPage,
            'firstPageNumber' => $this->firstPageNumber,
            'lastPage' => $this->lastPage,
            'lastPageNumber' => $this->lastPageNumber,
            'prevPage' => $this->prevPage,
            'prevPageNumber' => $this->prevPageNumber,
            'nextPage' => $this->nextPage,
            'nextPageNumber' => $this->nextPageNumber,
            'currentPage' => $this->currentPage,
            'currentPageNumber' => $this->currentPageNumber,
        );

        if ($returnPageNumbers) {
            $data['pagesNumber'] = array();
            for ($i = 0; $i < $this->pageCount; $i++) {
                $data['pagesNumber'][$i] = $i + 1;
            }
        }

        return $data;
    }

    /**
     * 产生指定范围内的页面索引和页号
     *
     * @param int $currentPage
     * @param int $navbarLen
     *
     * @return array
     */
    function getNavbarIndexs($currentPage = 0, $navbarLen = 8)
    {
        $mid = intval($navbarLen / 2);
        if ($currentPage < $this->firstPage) {
            $currentPage = $this->firstPage;
        }
        if ($currentPage > $this->lastPage) {
            $currentPage = $this->lastPage;
        }

        $begin = $currentPage - $mid;
        if ($begin < $this->firstPage) { $begin = $this->firstPage; }
        $end = $begin + $navbarLen - 1;
        if ($end >= $this->lastPage) {
            $end = $this->lastPage;
            $begin = $end - $navbarLen + 1;
            if ($begin < $this->firstPage) { $begin = $this->firstPage; }
        }

        $data = array();
        for ($i = $begin; $i <= $end; $i++) {
            $data[] = array('index' => $i, 'number' => ($i + 1 - $this->_basePageIndex));
        }
        return $data;
    }

    /**
     * 生成一个页面选择跳转控件
     *
     * @param string $caption
     * @param string $jsfunc
     */
    function renderPageJumper($caption = '%u', $jsfunc = 'fnOnPageChanged')
    {
        $out = "<select name=\"PageJumper\" onchange=\"{$jsfunc}(this.value);\">\n";
        for ($i = $this->firstPage; $i <= $this->lastPage; $i++) {
            $out .= "<option value=\"{$i}\"";
            if ($i == $this->currentPage) {
                $out .= " selected";
            }
            $out .=">";
            $out .= sprintf($caption, $i + 1 - $this->_basePageIndex);
            $out .= "</option>\n";
        }
        $out .= "</select>\n";
        echo $out;
    }

    /**
     * 计算各项分页参数
     */
    function computingPage()
    {
        $this->pageCount = ceil($this->count / $this->pageSize);
        $this->firstPage = $this->_basePageIndex;
        $this->lastPage = $this->pageCount + $this->_basePageIndex - 1;
        if ($this->lastPage < $this->firstPage) { $this->lastPage = $this->firstPage; }

        if ($this->lastPage < $this->_basePageIndex) {
            $this->lastPage = $this->_basePageIndex;
        }

        if ($this->currentPage >= $this->pageCount + $this->_basePageIndex) {
            $this->currentPage = $this->lastPage;
        }

        if ($this->currentPage < $this->_basePageIndex) {
            $this->currentPage = $this->firstPage;
        }

        if ($this->currentPage < $this->lastPage - 1) {
            $this->nextPage = $this->currentPage + 1;
        } else {
            $this->nextPage = $this->lastPage;
        }

        if ($this->currentPage > $this->_basePageIndex) {
            $this->prevPage = $this->currentPage - 1;
        } else {
            $this->prevPage = $this->_basePageIndex;
        }

        $this->firstPageNumber = $this->firstPage + 1 - $this->_basePageIndex;
        $this->lastPageNumber = $this->lastPage + 1 - $this->_basePageIndex;
        $this->nextPageNumber = $this->nextPage + 1 - $this->_basePageIndex;
        $this->prevPageNumber = $this->prevPage + 1 - $this->_basePageIndex;
        $this->currentPageNumber = $this->currentPage + 1 - $this->_basePageIndex;
    }
}
