package 
{
	import flash.events.Event;
	
	import org.god.Common.ClassLoader;
	//下载任务错误事件
	public class LoaderTaskErrorEvent extends Event
	{
		public static const EVENT_NAME:String = "LoaderTaskError";
		
		protected var m_cl:ClassLoader;//下载器
		//构造函数
		public function LoaderTaskErrorEvent(cl:ClassLoader)
		{
			super(EVENT_NAME);
			m_cl = cl;
		}
		//获取下载器
		public function GetClassLoader():ClassLoader
		{
			return m_cl;
		}
		//查询类定义
		public function GetClass():*
		{
			if(m_cl)
			{
				return m_cl.loader.content;
			}
			else
				return null;
		}
		//获取模块索引
		public function GetSWFModuleIndex():int
		{
			if(m_cl)
			{
				return m_cl.GetLoadContext().nTaskIndex;
			}
			else
				return -1;
		}
		//获取下载次数
		public function GetLoadCount():int
		{
			return m_cl.GetLoadCount();
		}
	}
}