package org.god.Common
{
	import org.god.IGameFrame.IMain;
	import org.god.IGameManagement.IMain;
	import org.god.Net.tagUserData;
	//全局主类
	public class GlobalCommonMain implements ICommonMain
	{
		protected var m_IGameFrameMain:org.god.IGameFrame.IMain;//游戏全局主类
		protected var m_IManagementMain:org.god.IGameManagement.IMain;//管理全局主类
		
		protected static var g_GlobalCommonMain:GlobalCommonMain;
		//构造函数
		public function GlobalCommonMain()
		{
		}
		//获取全局主类
		public static function getGlobalCommonMain():GlobalCommonMain
		{
			if(g_GlobalCommonMain == null)
				g_GlobalCommonMain = new GlobalCommonMain;
			return g_GlobalCommonMain;
		}
		//设置游戏全局主类
		public function SetIGameFrameMain(main:org.god.IGameFrame.IMain):void
		{
			m_IGameFrameMain = main;
		}
		//设置管理全局主类
		public function SetIGameManagementMain(main:org.god.IGameManagement.IMain):void
		{
			m_IManagementMain = main;
		}
		//获取游戏全局主类
		public function GetIGameFrameMain():org.god.IGameFrame.IMain
		{
			return m_IGameFrameMain;
		}
		//获取管理全局主类
		public function GetIGameManagementMain():org.god.IGameManagement.IMain
		{
			return m_IManagementMain;
		}
		//获取全局主类
		public function GetIMain():*
		{
			if(m_IGameFrameMain != null)
				return m_IGameFrameMain;
			return m_IManagementMain;
		}
		//获取用户信息
		public function GetUserData():tagUserData
		{
			if(m_IGameFrameMain)
				return m_IGameFrameMain.getUserData(); 
			else if(m_IManagementMain)
				return m_IManagementMain.getUserData(); 
			else
			return null;
		}
		//获取用户等级
		public function GetUserLevel():uint
		{
			if(m_IGameFrameMain)
				return m_IGameFrameMain.getUserData().cbLevel; 
			else if(m_IManagementMain)
				return m_IManagementMain.getUserData().cbLevel; 
			else
			return 0xff;
		}
		//获取用户游戏权限
		public function GetUserRight():uint
		{
			if(m_IGameFrameMain)
				return m_IGameFrameMain.getUserData().dwUserRight; 
			else if(m_IManagementMain)
				return m_IManagementMain.getUserData().dwUserRight; 
			else
			return 0xffffffff;
		}
		//获取用户管理权限
		public function GetMasterRight():uint
		{
			if(m_IGameFrameMain)
				return m_IGameFrameMain.getUserData().dwMasterRight; 
			else if(m_IManagementMain)
				return m_IManagementMain.getUserData().dwMasterRight; 
			else
			return 0xffffffff;
		}
		//获取用户类型
		public function GetUserType():uint
		{
			if(m_IGameFrameMain)
				return m_IGameFrameMain.getUserType(); 
			else if(m_IManagementMain)
				return m_IManagementMain.getUserType(); 
			else
			return 0xffffffff;
		}
	}
}