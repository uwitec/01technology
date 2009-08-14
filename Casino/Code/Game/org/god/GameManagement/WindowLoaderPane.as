package
{
	import flash.events.*;
	import flash.system.ApplicationDomain;
	import flash.display.MovieClip;
	
	import org.god.Common.ClassLoader;
	import org.god.SkinControl.SkinLabel;
	
	import org.aswing.JLabel;
	import org.aswing.ASColor;
	import org.aswing.util.HashMap;
	//SWF装载面板
	public class WindowLoaderPane extends MovieClip
	{
		public static const APPMODE_CHILD 	= ClassLoader.APPMODE_CHILD;//子应用域模式
		public static const APPMODE_CURRENT = ClassLoader.APPMODE_CURRENT;//当前应用域模式
		public static const APPMODE_NEW 	= ClassLoader.APPMODE_NEW;//新应用域模式

		private var m_loaderArray:Array;//类库装载器数组
		private var m_labelPercentArray:Array;//下载进度标签数组
		private var m_nLoadThreadCount:int;//下载线程数目
		private var m_SWFModuleMap:Array;//模块图
		private var m_nSWFModuleDepth:int;//模块深度
		private var m_nSWFModuleDepthIndex:int;//模块深度索引
		private var m_nSWFModuleCount:int;//模块数目
		private var m_nSWFModuleIndex:int;//模块索引
		private var m_labelSWFModuleStatus:SkinLabel;//模块状态标签
		
		private var m_pMain:Main;//主类
		//构造函数
		public function WindowLoaderPane(pMain:Main,
										 nSWFModuleDepth = 3,
										 nLoadThreadCount:int = 2)
		{
			m_pMain = pMain;
			m_nSWFModuleDepth  = nSWFModuleDepth;
			m_nLoadThreadCount = nLoadThreadCount;
			m_nSWFModuleDepthIndex = 0;
			m_nSWFModuleCount = 0;
			m_nSWFModuleIndex = 0;
			
			m_ProcessBar0.stop();
			m_ProcessBar1.stop();
			
			var MyCol:ASColor = new ASColor(0xCC9900);
			var processbar:MovieClip;
			m_loaderArray = new Array(nLoadThreadCount);
			m_labelPercentArray = new Array(nLoadThreadCount);
			for(var i:int = 0; i < nLoadThreadCount; i ++)
			{
				processbar = GetProcessBar(i);
				if(processbar == null)
					break;
				m_labelPercentArray[i] = new SkinLabel("",
										  processbar.x + processbar.width + 36, 
										   processbar.y - 3,
										   180);
				m_labelPercentArray[i].setHorizontalAlignment(JLabel.LEFT);
				addChild(m_labelPercentArray[i]);
				m_labelPercentArray[i].setForeground(MyCol);
			}
			processbar = GetProcessBar(nLoadThreadCount - 1);
			m_labelSWFModuleStatus = new SkinLabel("",
										  processbar != null? processbar.x:0, 
										   processbar != null? processbar.y + 30:0,
										   180);
			m_labelSWFModuleStatus.setHorizontalAlignment(JLabel.LEFT);
			addChild(m_labelSWFModuleStatus);
			m_labelSWFModuleStatus.setForeground(MyCol);
			m_SWFModuleMap = new Array(m_nSWFModuleDepth);
			for(i = 0; i < m_nSWFModuleDepth; i ++)
			{
				m_SWFModuleMap[i] = new HashMap;
			}
		}
		//销毁
		public function Destroy():void
		{
			for(var i:int = 0; i < m_nLoadThreadCount; i ++)
			{
				if(m_labelPercentArray[i] != null)
				{
					removeChild(m_labelPercentArray[i]);
					m_labelPercentArray[i] = null;
				}
				DestroyLoader(i);
			}
			m_labelPercentArray = null;
			m_loaderArray = null;
			removeChild(m_labelSWFModuleStatus);
			m_labelSWFModuleStatus = null;
			
			for(i = 0; i < m_nSWFModuleDepth; i ++)
			{
				m_SWFModuleMap[i].clear();
				m_SWFModuleMap[i] = null;
			}
			m_SWFModuleMap = null;
		}
		//添加下载任务
		public function AddLoaderTask(nSWFModuleDepthIndex:int,
									  nTaskIndex:int,
									  strName:String,
									  strURL:String, 
									  nAppMode:uint = 1):void
		{
			var task:LoaderTask = new LoaderTask;
			task.nTaskIndex 	= nTaskIndex;
			task.strName 		= strName;
			task.strURL 		= strURL;
			task.nAppMode 		= nAppMode;
			task.bAppModeFlag 	= true;
			m_SWFModuleMap[nSWFModuleDepthIndex].put(nTaskIndex,task);
			m_nSWFModuleCount ++;
		}
		//添加下载任务
		public function AddLoaderTaskInAppDomain(nSWFModuleDepthIndex:int,
												 nTaskIndex:int,
									  			 strName:String,
									  			 strURL:String, 
									 			 appDomain : ApplicationDomain = null):void
		{
			var task:LoaderTask = new LoaderTask;
			task.nTaskIndex 	= nTaskIndex;
			task.strName 		= strName;
			task.strURL		 	= strURL;
			task.appDomain 		= appDomain;
			task.bAppModeFlag 	= false;
			m_SWFModuleMap[nSWFModuleDepthIndex].put(nTaskIndex,task);
			m_nSWFModuleCount ++;
		}
		//开始下载
		public function StartLoad():void
		{
			var cl:ClassLoader;
			var index:int = 0;
			var task:LoaderTask;
			
			for(var i:int = 0; i < m_nLoadThreadCount; i ++)
			{
				task = GetNextTask();
				if(task != null)
				{
					index = i;
					cl = CreateLoader(index);
					task.bLoadingFlag = true;
					task.nLoaderIndex = index;
					cl.SetLoadContext(task);
					if(task.bAppModeFlag)
					{
						cl.loadInAppDomain(task.strURL, GetAppDomainByIndex(task.nAppMode));
					}
					else
					{
						if(task.appDomain == null)
							cl.loadInAppDomain(task.strURL, m_pMain.GetAppDomainByIndex(task.nTaskIndex));
						else
							cl.loadInAppDomain(task.strURL, task.appDomain);
					}

				}
			}
			
			SetSWFModuleStatus(m_nSWFModuleIndex,m_nSWFModuleCount);
		}
		//下载下一任务
		public function DoNextTask(cl:ClassLoader):void
		{
			//销毁任务
			var task:LoaderTask = cl.GetLoadContext();
			var index:int = task.nLoaderIndex;
			var bNewSWFModuleDepth:Boolean = RemoveTask(task);
			DestroyLoader(index);
			//执行新任务
			task = GetNextTask();
			if(task != null)
			{
				cl = CreateLoader(index);
				task.bLoadingFlag = true;
				task.nLoaderIndex = index;
				cl.SetLoadContext(task);
				if(task.bAppModeFlag)
				{
					cl.loadInAppDomain(task.strURL, GetAppDomainByIndex(task.nAppMode));
				}
				else
				{
					if(task.appDomain == null)
						cl.loadInAppDomain(task.strURL, m_pMain.GetAppDomainByIndex(task.nTaskIndex));
					else
						cl.loadInAppDomain(task.strURL, task.appDomain);
				}
				
				if(bNewSWFModuleDepth)
				{
					task = GetNextTask();
					index = GetFreeLoaderIndex();
					if(task != null && index != -1)
					{
						cl = CreateLoader(index);
						task.bLoadingFlag = true;
						task.nLoaderIndex = index;
						cl.SetLoadContext(task);
						if(task.bAppModeFlag)
						{
							cl.loadInAppDomain(task.strURL, GetAppDomainByIndex(task.nAppMode));
						}
						else
						{
							if(task.appDomain == null)
								cl.loadInAppDomain(task.strURL, m_pMain.GetAppDomainByIndex(task.nTaskIndex));
							else
								cl.loadInAppDomain(task.strURL, task.appDomain);
						}
					}
				}
			}
			m_nSWFModuleIndex ++;
			SetSWFModuleStatus(m_nSWFModuleIndex,m_nSWFModuleCount);
			
			if(m_nSWFModuleDepthIndex >= m_nSWFModuleDepth)
			{
				dispatchEvent(new LoaderTaskAllCompleteEvent());
			}
		}
		//重新下载
		public function DoTaskAgain(cl:ClassLoader):void
		{
			var task:LoaderTask = cl.GetLoadContext();
			var index:int = task.nLoaderIndex;
			var nLoadCount:int = cl.GetLoadCount();
			
			DestroyLoader(index);
			cl = CreateLoader(index);
			cl.AddLoadCount(nLoadCount + 1);
			
			task.bLoadingFlag = true;
			task.nLoaderIndex = index;
			cl.SetLoadContext(task);
			if(task.bAppModeFlag)
			{
				cl.loadInAppDomain(task.strURL, GetAppDomainByIndex(task.nAppMode));
			}
			else
			{
				if(task.appDomain == null)
					cl.loadInAppDomain(task.strURL, m_pMain.GetAppDomainByIndex(task.nTaskIndex));
				else
					cl.loadInAppDomain(task.strURL, task.appDomain);
			}
		}
		/////////////////////////////////////////////////////
		//设置下载状态
		private function SetSWFModuleStatus(cur:int,count:int):void
		{
			if(m_labelSWFModuleStatus)
			{
				m_labelSWFModuleStatus.setText("下载软体模块中: " + cur + " / " + count);
			}
		}
		//获取进度条影片
		private function GetProcessBar(index:int):MovieClip
		{
			switch(index)
			{
				case 0:
					return m_ProcessBar0;
				case 1:
					return m_ProcessBar1;
				default:
				break;
			}
			return null;
		}
		//创建类库装载器
		private function CreateLoader(index:int):ClassLoader
		{
			m_loaderArray[index] = new ClassLoader(false);
			m_loaderArray[index].addEventListener(ClassLoader.CLASS_LOADED, OnEventLoaded);
			m_loaderArray[index].addEventListener(ClassLoader.LOAD_ERROR, OnEventError);
			m_loaderArray[index].addEventListener(ClassLoader.LOAD_PROGRESS, OnEventProcess);
			return m_loaderArray[index];
		}
		//销毁类库装载器
		private function DestroyLoader(index:int):void
		{
			if(m_loaderArray[index] == null)
				return;
			m_loaderArray[index].Destroy();
			m_loaderArray[index].removeEventListener(ClassLoader.CLASS_LOADED, OnEventLoaded);
			m_loaderArray[index].removeEventListener(ClassLoader.LOAD_ERROR, OnEventError);
			m_loaderArray[index].removeEventListener(ClassLoader.LOAD_PROGRESS, OnEventProcess);
			m_loaderArray[index] = null;
		}
		//获取空闲类库装载器索引
		private function GetFreeLoaderIndex():int
		{
			for(var i:int = 0; i < m_loaderArray.length; i ++)
			{
				if(m_loaderArray[i] == null)
					return i;
			}
			return -1;
		}
		//设置进度
		private function SetPos(cl:ClassLoader,nPos:int):void
		{
			var index:int = cl.GetLoadContext().nLoaderIndex;
			if(nPos < 0)
			{
				nPos = 0;
			}
			else if(nPos > 100)
			{
				nPos = 100;
			}
			m_labelPercentArray[index].setText(cl.GetLoadContext().strName + " " +
											Number(nPos).toString() + "%");
			
			var processbar:MovieClip = GetProcessBar(index);
			if(processbar == null)
				return;
			processbar.gotoAndStop(nPos);
		}
		//获取下一下载任务
		private function GetNextTask():LoaderTask
		{
			if(m_nSWFModuleDepthIndex >= m_nSWFModuleDepth)
				return null;
			if(m_SWFModuleMap[m_nSWFModuleDepthIndex].isEmpty() == false)
			{
				var a:Array = m_SWFModuleMap[m_nSWFModuleDepthIndex].keys();
				for(var i:int = 0; i < a.length; i ++)
				{
					var task:LoaderTask = m_SWFModuleMap[m_nSWFModuleDepthIndex].get(a[i]);
					if(task.bLoadingFlag == false)
						return task;
				}

			}
			return null;
		}
		//清除下载任务
		private function RemoveTask(task:LoaderTask):Boolean
		{
			if(m_SWFModuleMap[m_nSWFModuleDepthIndex].isEmpty() == false)
			{
				m_SWFModuleMap[m_nSWFModuleDepthIndex].remove(task.nTaskIndex);
			}
			if(m_SWFModuleMap[m_nSWFModuleDepthIndex].isEmpty())
			{
				m_nSWFModuleDepthIndex ++;
				return true;
			}
			else
			{
				return false;
			}
		}
		//获取应用域
		private function GetAppDomainByIndex(nAppMode:int):ApplicationDomain
		{
			switch(nAppMode)
			{
				case APPMODE_CURRENT:
				{
					return m_pMain.GetAppCurrentDomain();
				}
				break;
				case APPMODE_NEW:
				{
					return m_pMain.GetAppNewDomain();
				}
				break;
				default:
				{
					return m_pMain.GetAppChildDomain();
				}
				break;
			}
			return null;
		}
		//下载完成事件
		private function OnEventLoaded(e:Event):void
		{
			//通知事件
			var cl:ClassLoader = e.currentTarget as ClassLoader;
			//dispatchEvent(new LoaderTaskCompleteEvent(cl));
			m_pMain.OnEventWindowLoadComplete(new LoaderTaskCompleteEvent(cl));
		}
		//下载进度事件
		private function OnEventProcess(e:ProgressEvent):void
		{
			var cl:ClassLoader = e.currentTarget as ClassLoader;
			var nPos:int = e.bytesLoaded * 100 / e.bytesTotal;
			this.SetPos(cl, nPos);
		}
		//下载错误事件
		private function OnEventError(e:Event):void
		{
			var cl:ClassLoader = e.currentTarget as ClassLoader;
			dispatchEvent(new LoaderTaskErrorEvent(cl));
		}

	}
}
include "../StringTableDef.as"
include "../GlobalConst.as"