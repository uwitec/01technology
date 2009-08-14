package 
{
	import flash.display.Sprite;
	import flash.display.Stage;
	import flash.display.DisplayObjectContainer;
	import flash.display.DisplayObject;
	import flash.display.StageScaleMode;
	import flash.utils.ByteArray;
	import flash.utils.Timer;
	import flash.system.*;
	import flash.events.Event;
	import flash.events.TimerEvent;
	import flash.events.FullScreenEvent;
	
	import org.aswing.util.LinkedList;
	import org.aswing.util.HashMap;
	import org.god.Net.IClientSocket;
	import org.god.Net.ClientSocket;
	import org.god.Net.tagUserData;
	import org.god.Net.GlobalNetError;
	import org.god.Net.CMD_GP_GameInfo;
	import org.god.Net.CMD_GP_Notice;
	import org.god.Common.AppDisplayState;
	import org.god.ISWFModule.IClassFactory;
	import org.god.ISWFModule.IModule;
	import org.god.IGameManagement.IMain;
	import org.god.IGameManagement.IPlazaWindow;
	import org.god.IGameManagement.ILogonWindow;
	import org.god.IGameManagement.UserDataChangeEvent;
	//主类
	public class Main extends Sprite implements IMain
	{
		public static const nStageWidth:int  = 1024;
		public static const nStageHeight:int = 768;
		
		public static const SWFModuleArray:Array = [
													"LogonWindow",
													"RuleWindow",
													"PlazaWindow",
													
													"MySelfDataPanel",
													"AccountPanel",
													"ChartPanel",
													"BetHistoryPanel",
													"BetMonitorPanel",
													"OperationPanel",
													
													"AccountPanel_Common",
													"BetHistoryPanel_Common"
													];
		public static const nSWFModuleIndex_Window:int = SWFModuleArray.indexOf("LogonWindow");
		public static const nSWFModuleIndex_Panel:int = SWFModuleArray.indexOf("MySelfDataPanel");
		public static const nSWFModuleIndex_Panel_Common:int = SWFModuleArray.indexOf("AccountPanel_Common");
		public static const nSWFModuleIndex_End:int = SWFModuleArray.indexOf("BetHistoryPanel_Common");

		private var m_ILogonWindow:ILogonWindow;//登陆窗口接口
		private var m_IPlazaWindow:IPlazaWindow;//大厅窗口接口
		private var m_nMainViewType:int = -1;//当前显示类型
		private var m_clientSocket:ClientSocket;//网络类
		private var m_UserData:tagUserData = new tagUserData;//用户信息
		private var m_GameInfo:CMD_GP_GameInfo = new CMD_GP_GameInfo;//游戏信息
		private var m_Notice:CMD_GP_Notice = new CMD_GP_Notice;//公告
		private var m_cbUserType:uint = 0;//用户类型
		private var m_wRealUserID:uint = 0;//用户真实ID
		private var m_bLogonPlaza:Boolean;//登陆大厅标记
		private var m_strAccount:String =  "";//用户帐号
		private var m_strPasswd:String =  "";//用户密码
		private var m_bScreenCheck:Boolean;//全屏检测
		private var m_timerFullScreenCheck:flash.utils.Timer;//全屏检测定时器
		private var m_arrModalDisplayObject:LinkedList;//模式化显示对象链表
		private var m_LoaderPane:WindowLoaderPane;//模块下载面板

		private var m_SWFModule:HashMap;//下载模块对应表
		//构造函数
		public function Main()
		{
			Security.allowDomain("*");
			System.useCodePage=true;
			//Security.loadPolicyFile("http://"+strWebMainServer+"/crossdomain.xml");
			stage.stageFocusRect = false;
			stage.scaleMode		 = StageScaleMode.SHOW_ALL;
			stage.addEventListener(Event.FULLSCREEN, OnEventFullScreen);
			
			var v:DisplayObject ;
			InitMain.Init(this);
			
			m_bLogonPlaza = false;
			
			m_arrModalDisplayObject= new LinkedList;
			m_timerFullScreenCheck = new flash.utils.Timer(4500, 1);
			m_timerFullScreenCheck.addEventListener("timer", OnEventTimer);
			m_clientSocket = new ClientSocket;
			
			m_SWFModule = new HashMap;

			StartLoadWindow();
		}
		//获取登陆窗口接口
		public function getILogonWindow():ILogonWindow
		{
			return m_ILogonWindow;
		}
		//获取大厅窗口接口
		public function getIPlazaWindow():IPlazaWindow
		{
			return m_IPlazaWindow;
		}
		//获取舞台宽度
		public function getStageWidth():int
		{
			return nStageWidth;
		}
		//获取舞台高度
		public function getStageHeight():int
		{
			return nStageHeight;
		}
		//获取当前显示类型
		public function SwitchMainViewType(nDM:int):void
		{
			if (m_nMainViewType == nDM)
			{
				if (nDM == nMainViewType_LogonWindow)
				{
					AppDisplayState.FocusFullScreen(false);
				}
				return;
			}
			else
			{
				switch (nDM)
				{
					case nMainViewType_LogonWindow :
						{
							AppDisplayState.FocusFullScreen(false);
							if(m_ILogonWindow != null)
							{
								getClientSocket().SetSocketSink(m_ILogonWindow.getSocketSink());
								m_ILogonWindow.Show();
							}
							if(m_IPlazaWindow != null)
							{
								m_IPlazaWindow.Hide();
							}
							
						}
						break;
					case nMainViewType_PlazaWindow :
						{
							AppDisplayState.FocusFullScreen(true);
							if(m_IPlazaWindow != null)
							{
								m_IPlazaWindow.Show();
							}
							if(m_ILogonWindow != null)
							{
								m_ILogonWindow.Hide();
							}
						};
						break;
					default :
						break;
				}
				m_nMainViewType = nDM;
			}
		}
		//重置
		public function resetAll():void
		{
			if(m_ILogonWindow != null)
			{
				m_ILogonWindow.resetAll();
			}
			if(m_IPlazaWindow != null)
			{
				m_IPlazaWindow.resetAll();
			}
			getClientSocket().CloseSocket(false);
			setLogonPlazaOK(false);
			m_UserData = new tagUserData;
		}
		//显示状态对话框
		public   function ShowStatusMessage(strMsg:String,bModal:Boolean=true):void
		{
			SkinProcessBox.ShowStatusMessage(strMsg);
		}
		//隐藏状态对话框
		public   function HideStatusMessage():void
		{
			SkinProcessBox.HideStatusMessage();
		}
		//显示消息对话框
		public   function ShowMessageBox(strMsg:String,bSplitMsg:Boolean = true,bModal:Boolean=true):void
		{
			SkinMessageBox.DoModalMessageBox(strMsg,bSplitMsg);
		}
		//显示错误信息对话框
		public function ShowErrorMessageBox(wErrCode:uint,bSplitMsg:Boolean = true,bModal:Boolean=true):void
		{
			ShowMessageBox(GetErrorDescribe(wErrCode),bSplitMsg,bModal);
		}
		//显示灰色信息对话框
		public   function ShowGrayMessageBox(strMsg:String,bModal:Boolean=true):void
		{
			SkinGrayMessageBox.DoModalMessageBox(strMsg);
		}
		//隐藏灰色信息对话框
		public   function HideGrayMessageBox():void
		{
			SkinGrayMessageBox.HideMessageBox();
		}
		//获取当前显示对象容器
		public function getActiveDOC():DisplayObjectContainer
		{
			switch (m_nMainViewType)
			{
				case nMainViewType_LogonWindow :
					{
						if(m_ILogonWindow != null)
							return m_ILogonWindow.getActiveDOC();
					}

				case nMainViewType_PlazaWindow :
					{
						if(m_IPlazaWindow != null)
						{
							return m_IPlazaWindow.getActiveDOC();
						}
					}

				default :
					return this;


			}
			return null;
		}
		//添加显示对象
		public function addChildDO(d:DisplayObject,
								   bModal:Boolean):void
		{
			addChild(d);
			if(bModal)
			{
				m_arrModalDisplayObject.append(d);
				
				var child:DisplayObject;
				var modalDO:DisplayObject;
				for(var i:int = 0;i < m_arrModalDisplayObject.size(); i ++)
				{
					modalDO = m_arrModalDisplayObject.get(i);
					
					for(var c:int = 0; c < this.numChildren; c++)
					{
						child = this.getChildAt(c);
						
						if(child != modalDO &&
						   getChildIndex(child) > getChildIndex(modalDO))
						{
							swapChildren(child,modalDO);
						}
					}
				}
			}
		}
		//删除显示对象
		public function removeChildDO(d:DisplayObject):void
		{
			removeChild(d);
			m_arrModalDisplayObject.remove(d);
		}
		//////////////////////////////////////////////
		//////////////////////////////////////////////
		//判断登陆大厅
		public function isLogonPlaza():Boolean
		{
			return m_bLogonPlaza;
		}
		//设置登陆大厅成功
		public function setLogonPlazaOK(bOK:Boolean):void
		{
			m_bLogonPlaza = bOK;				
			if(m_bLogonPlaza && m_IPlazaWindow)
			{
				getClientSocket().SetSocketSink(m_IPlazaWindow.getSocketSink());
				m_IPlazaWindow.OnEventFinishLogon();
			}
		}
		//设置用户信息
		public function setUserData(UserData:*):void
		{
			try
			{
				m_UserData = UserData as tagUserData;
				dispatchEvent(new UserDataChangeEvent(m_UserData));
			}
			catch (e:*)
			{
				throw Error("Bad UserData");
			}
		}
		//获取用户信息
		public function getUserData():tagUserData
		{
			return m_UserData;
		}
		//设置用户类型
		public function setUserType(cbUserType:uint):void
		{
			m_cbUserType = cbUserType;
		}
		//获取用户类型
		public function getUserType():uint
		{
			return m_cbUserType;
		}
		//设置用户真实ID
		public function setRealUserID(dwUserID:uint):void
		{
			m_wRealUserID = dwUserID;
		}
		//获取用户真实ID
		public function getRealUserID():uint
		{
			return m_wRealUserID;
		}
		//设置用户帐号
		public function setUserAccount(strAccount:String):void
		{
			m_strAccount = String(strAccount);
		}
		//获取用户帐号
		public function getUserAccount():String
		{
			return m_strAccount;
		}
		//设置用户密码
		public function setUserPasswd(strPasswd:String):void
		{
			m_strPasswd = String(strPasswd);
		}
		//获取用户密码
		public function getUserPasswd():String
		{
			return m_strPasswd;
		}
		//设置游戏信息
		public function setGameInfo(GameInfo:*):void
		{
			m_GameInfo = GameInfo;
		}
		//获取游戏信息
		public function getGameInfo():*
		{
			return m_GameInfo;
		}
		//设置公告
		public function setNotice(Notice:*):void
		{
			m_Notice = Notice;
		}
		//获取公告
		public function getNotice():*
		{
			return m_Notice;
		}
		//////////////////////
		//获取网络接口
		public function getClientSocket():IClientSocket
		{
			return m_clientSocket as IClientSocket;
		}
		//网络消息处理
		public function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			if(m_IPlazaWindow != null)
				return m_IPlazaWindow.OnSocketRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
			else
				return false;
		}
		
		///////////////////////////////////////////
		//定时器事件
		private function OnEventTimer(e:TimerEvent):void
		{
			if(m_bScreenCheck)
			{
				m_bScreenCheck = false;
				m_timerFullScreenCheck.stop();
				HideGrayMessageBox();
			}			
		}
		//全屏事件
		private function OnEventFullScreen(e:FullScreenEvent):void
		{
			if (e.fullScreen)
			{
				if(m_bScreenCheck == false)
				{
					m_bScreenCheck = true;
					m_timerFullScreenCheck.reset();
					m_timerFullScreenCheck.start();
					ShowGrayMessageBox(IDS_STATUS_FULLSCREEN);
					if(m_IPlazaWindow)
					{
						m_IPlazaWindow.OnEventToggleFullScreen(true);
					}
				}
			}
			else
			{
				if(m_bScreenCheck)
				{
					m_bScreenCheck = false;
					m_timerFullScreenCheck.stop();
					HideGrayMessageBox();
				}
				if(m_IPlazaWindow)
				{
					m_IPlazaWindow.OnEventToggleFullScreen(true);
				}
			}
		}
		//////////////////////////////////////////////////
		//获取应用域
		public function GetAppDomainByIndex(index:int):ApplicationDomain
		{
			return GetAppDomain(SWFModuleArray[index]);
		}
		//获取应用域
		public function GetAppDomain(strModuleName:String):ApplicationDomain
		{
			var module:IModule = m_SWFModule.get(strModuleName);
			if(module != null)
				return module.getApplicationDomain();
			else
			{
				var n:int = strModuleName.indexOf("_Common");
				if(n != -1)
				{
					var strTmp:String = strModuleName.substring(0, n);
					module = m_SWFModule.get(strTmp);
					if(module != null)
						return module.getApplicationDomain();
				}
				return new ApplicationDomain(ApplicationDomain.currentDomain);
			}
		}
		//获取当前应用域
		public function GetAppCurrentDomain():ApplicationDomain
		{
			return ApplicationDomain.currentDomain;
		}
		//获取新应用域
		public function GetAppNewDomain():ApplicationDomain
		{
			return new ApplicationDomain;
		}
		//获取子应用域
		public function GetAppChildDomain():ApplicationDomain
		{
			return new ApplicationDomain(ApplicationDomain.currentDomain);
		}
		//开始下载模块
		private function StartLoadWindow():void
		{
			m_LoaderPane = new WindowLoaderPane(this,3,2);
			addChild(m_LoaderPane);
			m_LoaderPane.x = 0;
			m_LoaderPane.y = 0;
			m_LoaderPane.addEventListener(LoaderTaskCompleteEvent.EVENT_NAME,
										 OnEventWindowLoadComplete); 
			m_LoaderPane.addEventListener(LoaderTaskErrorEvent.EVENT_NAME,
										 OnEventWindowLoadError); 
			m_LoaderPane.addEventListener(LoaderTaskAllCompleteEvent.EVENT_NAME,
										 OnEventWindowLoadAllComplete); 
			for(var i:int = 0; i < SWFModuleArray.length; i ++)
			{
				if(i < nSWFModuleIndex_Panel)
				{
					m_LoaderPane.AddLoaderTaskInAppDomain(0,
											   i,
											   SWFModuleArray[i],
											   SWFModuleArray[i]+ ".swf",
											   GetAppCurrentDomain());

				}
				else if(i >= nSWFModuleIndex_Panel &&
							i < nSWFModuleIndex_Panel_Common)
				{
					m_LoaderPane.AddLoaderTaskInAppDomain(1,
											   i,
											   SWFModuleArray[i],
											   SWFModuleArray[i]+ ".swf",
											   GetAppChildDomain());
					
				}
				else if(i >= nSWFModuleIndex_Panel_Common)
				{
					m_LoaderPane.AddLoaderTaskInAppDomain(2,
											   i,
											   SWFModuleArray[i],
											    SWFModuleArray[i]+ ".swf",
											   null);
				}
			}
			
			m_LoaderPane.StartLoad();
		}
		//停止下载模块
		private function StopLoadWindow():void
		{
			if(m_LoaderPane)
			{
				m_LoaderPane.removeEventListener(LoaderTaskCompleteEvent.EVENT_NAME,
										 OnEventWindowLoadComplete); 
				m_LoaderPane.removeEventListener(LoaderTaskErrorEvent.EVENT_NAME,
										 OnEventWindowLoadError); 
				m_LoaderPane.removeEventListener(LoaderTaskAllCompleteEvent.EVENT_NAME,
										 OnEventWindowLoadAllComplete);
	
				m_LoaderPane.Destroy();
				removeChild(m_LoaderPane);
				m_LoaderPane = null;
			}
		}
		//下载模块错误事件
		public function OnEventWindowLoadError(e:LoaderTaskErrorEvent):Boolean
		{
			//ShowMessageBox(IDS_ERR_LOAD);
			//StopLoadWindow();
			trace("下载模块错误 SWFModuleIndex:" + e.GetSWFModuleIndex());
			if(e.GetLoadCount() < 5)
			{
				m_LoaderPane.DoTaskAgain(e.GetClassLoader());
				trace("重新下载");
				return false;
			}
			else
			{
				ShowMessageBox(IDS_ERR_LOAD);
				return true;
			}
		}
		//下载模块完成事件
		public function OnEventWindowLoadComplete(e:LoaderTaskCompleteEvent):Boolean
		{
			var module:IModule = e.GetClass() as IModule;
			if(module)
			{
				var index:int = e.GetSWFModuleIndex();
				
				m_SWFModule.put(SWFModuleArray[index], module);
				m_LoaderPane.DoNextTask(e.GetClassLoader());
				
				return true;
			}
			else
			{
				trace("下载模块完毕 查询接口失败 SWFModuleIndex:" + e.GetSWFModuleIndex());
				if(e.GetLoadCount() < 5)
				{
					m_LoaderPane.DoTaskAgain(e.GetClassLoader());
					trace("重新下载");
					return false;
				}
				else
				{
					ShowMessageBox(IDS_ERR_LOAD);
					return true;
				}
			}
		}
		//下载全部模块完成事件
		public function OnEventWindowLoadAllComplete(e:LoaderTaskAllCompleteEvent):void
		{
			if(m_ILogonWindow == null)
			{
				var c:Class;
				c = getDefClass("LogonWindow","LogonWindow");
				if(c != null)
				{
					m_ILogonWindow = new c(this);
				}
				c = getDefClass("PlazaWindow","PlazaWindow");
				if(c != null)
				{
					m_IPlazaWindow = new c(this);
				}
				
			}
			if(m_ILogonWindow != null)
			{
				StopLoadWindow();
				addChildDO(m_ILogonWindow.getActiveDOC(),false);
				addChildDO(m_IPlazaWindow.getActiveDOC(),false);
			
				SwitchMainViewType(nMainViewType_LogonWindow);
			}
			else
			{
				ShowMessageBox(IDS_ERR_LOAD);
			}
		}
		//添加用户信息修改监听
		public function addUserDataChangeEventListener(listener:Function):void
		{
			addEventListener(UserDataChangeEvent.strEventName,listener);
		}
		//删除用户信息修改监听
		public function removeUserDataChangeEventListener(listener:Function):void
		{
			removeEventListener(UserDataChangeEvent.strEventName,listener);
		}
		//获取错误文本
		public function GetErrorDescribe(wErrCode:uint):String
		{
			return GlobalNetError.GetErrorDescribe(wErrCode);
		}
		//获取定义类
		public function getDefClass(strClassName:String,
									strModuleName:String):Class
		{
			var c:Class = null;
			var module:IModule = m_SWFModule.get(strModuleName);
			if(module != null)
			{
				c = module.GetClass(strClassName);
				if(c != null)
					return c;
			}
			
			c = ApplicationDomain.currentDomain.getDefinition(strClassName) as Class;
			return c;
			
		}
		
	}
}
include "../Globalconst.as"
include "../StringTableDef.as"
include "../Net/CMD_Plaza.as"
include "../Net/GLOBALDEF.as"
