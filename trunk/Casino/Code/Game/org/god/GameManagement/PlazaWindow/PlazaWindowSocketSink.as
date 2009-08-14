package 
{
	import flash.utils.ByteArray;
	import org.god.Net.IClientSocketSink;
	import org.god.Net.IClientSocket;
	import org.god.Net.IUnknownEx;
	//大厅socket处理
	public class PlazaWindowSocketSink  implements IClientSocketSink,IUnknownEx
	{
		private var m_mcParent:PlazaWindow;
		public function PlazaWindowSocketSink(mcParent:PlazaWindow)
		{
			m_mcParent = mcParent;
		}
		//是否有效
		public function IsValid():Boolean
		{
			return true;
		}		//释放对象
		public function Release():Boolean
		{
			return true;
		}
		//接口查询
		public function QueryInterface(Guid:String,dwQueryVer:uint):*
		{
			if (Guid == IID_IUnknownEx)
			{
				return this;
			}
			else if (Guid == IID_IClientSocketSink)
			{
				return this;
			}
			return null;
		}
		//网络连接消息
		public function OnSocketConnect(iErrorCode:int,pszErrorDesc:String,pIClientSocke:IClientSocket):Boolean
		{	
			return m_mcParent.OnSocketEventConnect(iErrorCode,pszErrorDesc,pIClientSocke);
		}

		//网络读取消息
		public function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocke:IClientSocket):Boolean
		{
			return m_mcParent.OnSocketEventRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocke);
		}
		//网络关闭消息
		public function OnSocketClose(pIClientSocke:IClientSocket,bCloseByServer:Boolean):Boolean
		{
			return m_mcParent.OnSocketEventClose(pIClientSocke,bCloseByServer);
		}
	}
}
include "../../Net/InterfaceModuleIDef.as"
include "../../Net/NetModuleIDef.as"
