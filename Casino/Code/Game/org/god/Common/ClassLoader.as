package org.god.Common
{
	import flash.display.Loader;
	import flash.display.LoaderInfo;
	import flash.errors.IllegalOperationError;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IOErrorEvent;
	import flash.events.SecurityErrorEvent;
	import flash.events.ProgressEvent;
	import flash.net.URLRequest;
	import flash.system.System;
	import flash.system.LoaderContext;
	import flash.system.ApplicationDomain;
	
	import org.god.IGameFrame.IGameClass;
	import org.god.IGameFrame.IGameModule;
	//类库装载器
	public class ClassLoader extends EventDispatcher
	{
		public static const APPMODE_CHILD = 0;//子应用域模式
		public static const APPMODE_CURRENT = 1;//当前应用域模式
		public static const APPMODE_NEW = 2;//新应用域模式
		
		public static var CLASS_LOADED:String = "classLoaded";//下载完毕事件
		public static var LOAD_ERROR:String = "loadError";//下载失败事件
		public static var LOAD_PROGRESS:String = ProgressEvent.PROGRESS;//下载进度事件
		
		public  var loader:Loader;//下载器
		private var swfLib:String;//下载类名
		private var request:URLRequest;//资源请求
		private var loadedClass:Class;//类
		private var module:IGameModule;//游戏模块接口
		private var _bAutoHandleGameModule:Boolean;//自动处理游戏模块接口
		private var m_loadContext:*;//下载上下文
		public 	var m_nLoadCount:int;//下载次数
		//构造函数
		public function ClassLoader(bAutoHandleGameModule:Boolean=true)
		{
			m_nLoadCount = 0;
			_bAutoHandleGameModule = bAutoHandleGameModule;
			InitFirstHandler();
		}
		//销毁
		public function Destroy():void
		{
			UninitFirstHandler();
			//loader = null;
			swfLib = "";
			request = null;
			loadedClass = null;
			module = null;
			//m_loadContext = null;
		}
		//获取下载次数
		public function GetLoadCount():int
		{
			return m_nLoadCount;
		}
		//增加下载次数
		public function AddLoadCount(n:int):void
		{
			 m_nLoadCount += n;
		}
		//设置下载上下文
		public function SetLoadContext(context:*):void
		{
			m_loadContext = context;
		}
		//获取下载上下文
		public function GetLoadContext():*
		{
			return m_loadContext;
		}
		//初始化下载器
		private function InitFirstHandler():void
		{
			loader = new Loader();
			loader.contentLoaderInfo.addEventListener(Event.INIT,completeFirstHandler);
			loader.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR,ioErrorHandler);
			loader.contentLoaderInfo.addEventListener(SecurityErrorEvent.SECURITY_ERROR,securityErrorHandler);
			loader.contentLoaderInfo.addEventListener(ProgressEvent.PROGRESS,progressHandler);
		}
		//初始化下载器
		private function InitSecondHandler():void
		{
			loader = new Loader();
			loader.contentLoaderInfo.addEventListener(Event.INIT,completeSecondHandler);
			loader.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR,ioErrorHandler);
			loader.contentLoaderInfo.addEventListener(SecurityErrorEvent.SECURITY_ERROR,securityErrorHandler);
			loader.contentLoaderInfo.addEventListener(ProgressEvent.PROGRESS,progressHandler);
		}
		//释放下载器
		private function UninitFirstHandler():void
		{
			loader.contentLoaderInfo.removeEventListener(Event.INIT,completeFirstHandler);
			loader.contentLoaderInfo.removeEventListener(IOErrorEvent.IO_ERROR,ioErrorHandler);
			loader.contentLoaderInfo.removeEventListener(SecurityErrorEvent.SECURITY_ERROR,securityErrorHandler);
			loader.contentLoaderInfo.removeEventListener(ProgressEvent.PROGRESS,progressHandler);
		}
		//再次下载
		private function loadAgain():void
		{
			UninitFirstHandler();
			loader = null;
			InitSecondHandler();
			load(swfLib);
			m_nLoadCount ++;
		}
		//下载
		public function load(lib:String, nAppMode:uint = 0):void
		{
			swfLib = lib;
			request = new URLRequest(swfLib);
			var context:LoaderContext = new LoaderContext();
			switch(nAppMode)
			{
				case APPMODE_CURRENT:
				{
					context.applicationDomain = ApplicationDomain.currentDomain;
				}
				break;
				case APPMODE_NEW:
				{
					context.applicationDomain = new ApplicationDomain();
				}
				break;
				default:
				{
					context.applicationDomain = new ApplicationDomain(ApplicationDomain.currentDomain);
				}
				break;
			}
			loader.load(request,context);
		}
		//下载
		public function loadInAppDomain(lib:String, appDomain : ApplicationDomain = null):void
		{
			swfLib = lib;
			request = new URLRequest(swfLib);
			var context:LoaderContext = new LoaderContext();
			context.applicationDomain = appDomain;
			loader.load(request,context);
		}
		//获取类
		public function getClass(className:String):Class
		{
			try
			{
				return loader.contentLoaderInfo.applicationDomain.getDefinition(className)  as  Class;
			}
			catch (e:Error)
			{
				System.setClipboard(e.toString());
				throw new IllegalOperationError(className + " definition not found in "  + e.toString());
			}
			return null;
		}
		//获取游戏类接口
		public function newDefaultIGameClass():IGameClass
		{
			try
			{
				if(module)
				{
					return module.getIGameClass();
				}
				else
				{
					module = loader.contentLoaderInfo.content as IGameModule;
					if(module)
					{
						return module.getIGameClass();
					}
					else
					{
						throw new IllegalOperationError("newDefaultIGameClass: IGameModule is null");
					}
				}
			}
			catch (e:Error)
			{
				System.setClipboard(e.toString());
				throw new IllegalOperationError(e.toString());
			}
			return null;
		}
		//下载完成
		private function completeFirstHandler(e:Event):void
		{
			if(_bAutoHandleGameModule)
			{
				if(e.currentTarget)
				{
					var info : LoaderInfo = e.currentTarget as LoaderInfo;
					
					if(info && info.content)
					{
						module = info.content as IGameModule;
					}
				}
				if(module == null)
				{
					System.setClipboard("completeHandler First:IGameModule is null");
					loadAgain();
					return ;
				}
			}
			dispatchEvent(new Event(ClassLoader.CLASS_LOADED));
		}
		//下载完成
		private function completeSecondHandler(e:Event):void
		{
			if(_bAutoHandleGameModule)
			{
				if(e.currentTarget)
				{
					var info : LoaderInfo = e.currentTarget as LoaderInfo;
					if(info && info.content)
					{
						module = info.content as IGameModule;
					}
				}
				if(module == null)
				{
					System.setClipboard("completeHandler Second:IGameModule is null");
					dispatchEvent(new Event(ClassLoader.LOAD_ERROR));
					return ;
				}
			}
			dispatchEvent(new Event(ClassLoader.CLASS_LOADED));
		}
		//错误处理
		private function ioErrorHandler(e:Event):void
		{
			dispatchEvent(new Event(ClassLoader.LOAD_ERROR));
		}
		//错误处理
		private function securityErrorHandler(e:Event):void
		{
			dispatchEvent(new Event(ClassLoader.LOAD_ERROR));
		}
		//下载进度处理
		private function progressHandler(e:ProgressEvent):void
		{
			dispatchEvent(e.clone());
		}
	}
}