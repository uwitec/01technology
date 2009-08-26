<?php
class Controller_Admin extends FLEA_Controller_Action
{

	var $_modelNews;
	var $_modelMessage;
	
    /**
     * 构造函数
     */
    function Controller_Admin()
    {
        /**
         * FLEA::getSingleton() 会自动载入指定类的定义文件，并且返回该类的唯一一个实例
         */
        $this->_modelNews =& FLEA::getSingleton('Model_News');
        $this->_modelMessage =& FLEA::getSingleton('Model_Message');
        $this->_modelGoods =& FLEA::getSingleton('Model_Goods');
        $this->_modelCategory =& FLEA::getSingleton('Model_Category');
        $this->_modelAdminUser =& FLEA::getSingleton('Model_AdminUser');
    }
    
    /**
     * 登录检查
     */
    function LoginCheck() {
    	if (!isset($_SESSION['_LOGINID_']) OR $_SESSION['_LOGINID_'] != ADMIN_NAME) {
    		echo "请登录！";
    		sleep(3);
	        redirect($this->_url("Index"));
    		exit;
    	}
    }
	
	/**
     * 默认控制器方法
     */
    function actionIndex() {
        $smarty =& $this->_getView();
        /* @var $smarty Smarty */
        $smarty->display('tpl-adminlogin.html');
    }
    
	/**
	 * 登录
	 */
    function actionLogin() {
    	
    	if (isset($_POST['name']) AND isset($_POST['pwd'])) {
	    	$count = $this->_modelAdminUser->findCount("`name` = '" . $_POST['name']. "' AND `password` = '" . $_POST['pwd'] . "' AND `delflg` = 0");
    		if (1 == $count) {
		    	$_SESSION['_LOGINID_'] = $_POST['name'];
	    		redirect($this->_url("Newslist"));
				exit;
    		}else {
	    		$smarty =& $this->_getView();
	    		$smarty->display("tpl-error.html");
    		}
    	}else {
    		$smarty =& $this->_getView();
    		$smarty->display("tpl-error.html");
    	}
    }

    /**
     * 退出
     */
    function actionLogout() {
    	unset($_SESSION['_LOGINID_']);
   		redirect($this->_url("Index"));
		exit;
    }

    /**
     * 更改密码
     */
    function actionChangePSW() {
	    $smarty =& $this->_getView();
	    $smarty->display("tpl-admin-changepsw.html");
    }
    
    function actionChangePSWdo() {
    	if (1 == $this->_modelAdminUser->findCount("`name` = '" . $_SESSION['_LOGINID_']. "' AND `password` = '" . $_POST['password'] . "' AND `delflg` = 0") AND 
    		$_POST['passwordnew'] == $_POST['pssswordrepet']) {
    		$row = $this->_modelAdminUser->find("`name` = '" . $_SESSION['_LOGINID_']. "' AND `password` = '" . $_POST['password'] . "' AND `delflg` = 0");
    		unset($row['name']);
    		unset($row['delflg']);
    		$row['password'] = $_POST['passwordnew'];
    		$this->_modelAdminUser->save($row);
    		redirect($this->_url("Logout"));
    		exit;
    	}else {
    		$smarty =& $this->_getView();
    		$smarty->display("tpl-error.html");
    	}
    }
    
    /**
     * 新闻列表
     *
     */
    function actionNewslist() {
    	// 登录检查
    	$this->LoginCheck();
    	
		// 装载Pager类
		FLEA::loadClass('FLEA_Helper_Pager');

		$smarty =& $this->_getView();
		
		$page = (isset($_GET['page']))?(int)$_GET['page']:1;
		$pageSize = 10;
		$conditions = "`delflg` = 0";
		$sort = "`newsid` DESC";
		$pager = & new FLEA_Helper_Pager($this->_modelNews, $page, $pageSize, $conditions, $sort);

		$pager->setBasePageIndex(1);  // 起始页码设为1
		$rows = $pager->findAll();
		$smarty->assign('Navbar', $pager->getNavbarIndexs($page, 8));
		$smarty->assign('Pager', $pager->getPagerData());
		$url = array('ctl'=>'Admin','act'=>'Newslist');
		$smarty->assign('url',$url);
		
		// 列表显示新闻标题
		$smarty->assign('news',$rows);
		
    	$smarty->display('tpl-admin-newslist.html');
    }
    
    /**
     * 删除新闻
     */
    function actionNewsdel() {
    	// 登录检查
    	$this->LoginCheck();
    	
    	if (!isset($_GET['id'])) {
    		redirect($this->_url("Newslist"));
    		exit;
    	}
    	$row['newsid'] = $_GET['id'];
    	$row['delflg'] = 1;
    	if ($this->_modelNews->save($row)) {
    		redirect($this->_url("Newslist"));
    		exit;
    	}
    }
    
    /**
     * 编辑/添加新闻
     */
    function actionNewsedit() {
    	// 登录检查
    	$this->LoginCheck();
    	
    	if (!isset($_GET['id'])) {
    		$news = array();
    	}else {
    		$news = $this->_modelNews->find("`newsid` =" . $_GET['id']);
    	}
    	$smarty =& $this->_getView();
    	
		$smarty->assign('news',$news);
		$smarty->display('tpl-admin-newsedit.html');
    }
    
    /**
     * 编辑/添加新闻处理
     */
    function actionNewseditdo() {
    	// 登录检查
    	$this->LoginCheck();
    	
    	$row['title'] = $_POST['title'];
    	$row['content'] = $_POST['content']; 
    	$row['type'] = $_POST['type'];
    	$row['author'] = $_SESSION['_LOGINID_'];
    	if (isset($_POST['newsid'])) {
    		$row['newsid'] = $_POST['newsid'];
	    	$row['updated'] = date("Y-m-d H:i:s");
    	}else {
    		$row['created'] = date("Y-m-d H:i:s");
    	}
    	if ($this->_modelNews->save($row)) {
    		redirect($this->_url("Newslist"));
    		exit;
    	}
    }
    
    /**
     * 留言列表
     */
    function actionMsglist() {
    	// 登录检查
    	$this->LoginCheck();
    	
		// 装载Pager类
		FLEA::loadClass('FLEA_Helper_Pager');

		$smarty =& $this->_getView();
		
		$page = (isset($_GET['page']))?(int)$_GET['page']:1;
		$pageSize = 10;
		$conditions = "`delflg` = 0";
		$sort = "`created` DESC";
		$pager = & new FLEA_Helper_Pager($this->_modelMessage, $page, $pageSize, $conditions, $sort);
		
		$pager->setBasePageIndex(1);  // 起始页码设为1
		$rows = $pager->findAll();
		$smarty->assign('Navbar', $pager->getNavbarIndexs($page, 8));
		$smarty->assign('Pager', $pager->getPagerData());
		$url = array('ctl'=>'Admin','act'=>'Msglist');
		$smarty->assign('url',$url);
		
		// 列表显示留言标题
		$smarty->assign('msgs',$rows);
		
    	$smarty->display('tpl-admin-msglist.html');
    	
    }
    
    /**
     * 删除留言
     */
    function actionMsgdel() {
    	// 登录检查
    	$this->LoginCheck();
    	
    	if (!isset($_GET['id'])) {
    		redirect($this->_url("Msglist"));
    		exit;
    	}
    	$row['msgid'] = $_GET['id'];
    	$row['delflg'] = 1;
    	if ($this->_modelMessage->save($row)) {
    		redirect($this->_url("Msglist"));
    		exit;
    	}
    }
    
    /**
     * 查看留言
     */
    function actionMsgshow() {
    	// 登录检查
    	$this->LoginCheck();
    	
    	if (!isset($_GET['id'])) {
    		redirect($this->_url("Msglist"));
    		exit;
    	}
    	$row['msgid'] = $_GET['id'];
    	$row['viewed'] = 1;
    	$this->_modelMessage->save($row);
    	$row = $this->_modelMessage->find("`msgid` = " . $_GET['id']);

    	$smarty =& $this->_getView();
    	
    	$smarty->assign('row', $row);
		$smarty->display('tpl-admin-msgshow.html');
    }
    
    /**
     * 产品列表
     */
    function actionGoodslist() {
    	// 登录检查
    	$this->LoginCheck();
    	
		// 装载Pager类
		FLEA::loadClass('FLEA_Helper_Pager');

		$smarty =& $this->_getView();
		
		$page = (isset($_GET['page']))?(int)$_GET['page']:1;
		$pageSize = 4;
		$conditions = "`delflg` = 0";
		$sort = null;
		$pager = & new FLEA_Helper_Pager($this->_modelGoods, $page, $pageSize, $conditions, $sort);
		
		$pager->setBasePageIndex(1);  // 起始页码设为1
		$rows = $pager->findAll();
		$smarty->assign('Navbar', $pager->getNavbarIndexs($page, 8));
		$smarty->assign('Pager', $pager->getPagerData());
		$url = array('ctl'=>'Admin','act'=>'Goodslist');
		$smarty->assign('url',$url);
		
		// 列表显示留言标题
		$smarty->assign('goods',$rows);
		
    	$smarty->display('tpl-admin-goodslist.html');
    	
    }
    
    /**
     * 删除产品
     */
    function actionGoodsdel() {
    	// 登录检查
    	$this->LoginCheck();
    	
    	if (!isset($_GET['id'])) {
    		redirect($this->_url("Goodslist"));
    		exit;
    	}
    	$row['goodsid'] = $_GET['id'];
    	$row['delflg'] = 1;
    	if ($this->_modelGoods->save($row)) {
    		redirect($this->_url("Goodslist"));
    		exit;
    	}
    }
    
    /**
     * 添加/修改产品
     */
    function actionGoodsedit() {
    	// 登录检查
    	$this->LoginCheck();
    	
    	if (!isset($_GET['id'])) {
    		$goods = array('cateid' => null);
    	}else {
    		$goods = $this->_modelGoods->find("`goodsid` =" . $_GET['id']);
    	}
    	
    	$categorys = $this->_modelCategory->findAll();

		$html_select = "<select name=\"cateid\">\n";
	    foreach ($categorys as $key => $value) {
	        $html_select .= '<option value="' . h($value['cateid']) . '"';
	        if ($goods['cateid'] == $value['cateid']) { $html_select .= ' selected'; }
	        $html_select .= '>' . h($value['title']) . "</option>\n";
	    }
	    $html_select .= "</select>\n";
	    
		$smarty =& $this->_getView();
		$smarty->assign('goods',$goods);
		$smarty->assign('html_select',$html_select);
		$smarty->display('tpl-admin-goodsedit.html');
    }
    
    /**
     * 添加/修改产品Action
     */
    function actionGoodseditdo() {
    	// 登录检查
    	$this->LoginCheck();
    	
    	$row['name'] = $_POST['name'];
		$row['summary'] = $_POST['summary'];    	
    	$row['detail'] = $_POST['detail']; 
    	$row['cateid'] = $_POST['cateid'];
    	$row['author'] = $_SESSION['_LOGINID_'];
    	if ($_FILES['image']['error'][0] != 4) {
			$allowExts = ALLOW_EXTS;
			$maxSize = MAX_FILES;
			$uploadDir = UPLOAD_IMG_DIR;
			
			// 装载FileUploader类
			FLEA::loadClass('FLEA_Helper_FileUploader');
			$uploader =& new FLEA_Helper_FileUploader();
			$files =& $uploader->getFiles();
			foreach ($files as $file) {
			    if (!$file->check($allowExts, $maxSize)) {
			        // 上传的文件类型不符或者超过了大小限制。
			        return false;
			    }
			    // 生成唯一的文件名（重复的可能性极小）
			    $id = md5(time() . $file->getFilename() . $file->getSize() . $file->getTmpName());
			    $filename = $id . '.' . strtolower($file->getExt());
			    $file->move($uploadDir . '/' . $filename);
			}
			$row['image'] = $filename;
    	}
    	if (isset($_POST['goodsid'])) {
    		$row['goodsid'] = $_POST['goodsid'];
	    	$row['updated'] = date("Y-m-d H:i:s");
    	}else {
    		$row['created'] = date("Y-m-d H:i:s");
    	}
    	if ($this->_modelGoods->save($row)) {
    		redirect($this->_url("Goodslist"));
    		exit;
    	}
    }
}


